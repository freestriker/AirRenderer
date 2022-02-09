#include "MeshRenderer.h"
#include "Configuration.h"
#include <QDebug>
#include <qdir.h>
#include <QCoreApplication>
#include "FaceContext.h"
#include "PixelInContext.h"
#include "PixelOutContext.h"
#include "PixelIterator.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "VertexInContext.h"
#include "VertexOutContext.h"
#include "FaceContext.h"

MeshRenderer::MeshRenderer()
{
	std::string s = (QCoreApplication::applicationDirPath() + '/' + "../../Model/Triangle.obj").toStdString();
	qDebug() << "Current working directory: " << QCoreApplication::applicationDirPath() << endl;
    OpenMesh::IO::Options o = OpenMesh::IO::Options::ColorFloat + OpenMesh::IO::Options::VertexColor + OpenMesh::IO::Options::ColorAlpha;
    mesh.request_vertex_colors();
	if (!OpenMesh::IO::read_mesh(mesh, s, o, true))
	{
		qDebug() << "Failed to load " << QString::fromStdString(s) << endl;
		exit(1);
	}
    shader = Shader();
}

void MeshRenderer::Render(glm::mat4 mvpMatrix, glm::mat4 screenMatrix)
{
    std::vector<VertexOutContext> vertexOutContext = std::vector<VertexOutContext>(mesh.vertices_end().handle().idx(), VertexOutContext());
    for (Mesh::VertexIter v_it = mesh.vertices_sbegin(); v_it != mesh.vertices_end(); v_it++)
    {
        int index = (*v_it).idx();
        Mesh::Point p = mesh.point(*v_it);
        Mesh::Color c = mesh.vertex_colors()[index] / 255.0;
        VertexInContext vertexInContext = VertexInContext();
        vertexInContext.position = glm::vec4(p[0], p[1], p[2], 1);
        vertexInContext.color = Color(c[0], c[1], c[2], 1);
        vertexInContext.mvpMatrix = mvpMatrix;
        vertexInContext.vertexIndex = index;

        //顶点着色器
        shader.VertexShading(vertexInContext, vertexOutContext[index]);
    }
    for (Mesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
    {
        FaceContext faceContext = FaceContext();
        int index = 0, inBoundryCount = 0;
        for (Mesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
        {
            int vertexIndex = (*fv_it).idx();
            glm::vec4 pos = vertexOutContext[vertexIndex].position;

            //测试剔除
            if (-pos.w < pos.x && pos.x < pos.w
                && -pos.w < pos.y && pos.y < pos.w
                && 0 < pos.z && pos.z < pos.w)
            {
                inBoundryCount++;
            }

            //透视除法
            float w = pos.w;
            pos = pos / w;

            //光栅化
            pos = screenMatrix * pos;

            //图元装配
            faceContext.screenPosition[index] = glm::ivec2(pos.x + 0.5, pos.y + 0.5);
            faceContext.z[index] = pos.z;
            faceContext.w[index] = pos.w;
            faceContext.vertexIndex[index] = vertexIndex;
            index++;
        }
        if (inBoundryCount > 0)
        {
            for (PixelIterator pixelIterator = PixelIterator(faceContext); pixelIterator.CheckValid(); pixelIterator++)
            {
                glm::ivec2 screenPosition = pixelIterator.GetScreenPosition();

                if (0 <= screenPosition.x && screenPosition.x < configuration.resolution.width
                    && 0 <= screenPosition.y && screenPosition.y < configuration.resolution.height)
                {
                    glm::dvec3 barycentricPosition = pixelIterator.GetBarycentricCoordinates();
                    PixelInContext pixelInContext = PixelInContext();
                    PixelOutContext pixelOutContext = PixelOutContext();
                    pixelInContext.screenPosition = screenPosition;
                    
                    pixelInContext.position = vertexOutContext[faceContext.vertexIndex[0]].position * float(barycentricPosition.x) + vertexOutContext[faceContext.vertexIndex[1]].position * float(barycentricPosition.y) + vertexOutContext[faceContext.vertexIndex[2]].position * float(barycentricPosition.z);
                    pixelInContext.color = vertexOutContext[faceContext.vertexIndex[0]].color * barycentricPosition.x + vertexOutContext[faceContext.vertexIndex[1]].color * barycentricPosition.y + vertexOutContext[faceContext.vertexIndex[2]].color * barycentricPosition.z;
                    pixelInContext.w = faceContext.w[0] * barycentricPosition.x + faceContext.w[1] * barycentricPosition.y + faceContext.w[2] * barycentricPosition.z;
                    pixelInContext.z = faceContext.z[0] * barycentricPosition.x + faceContext.z[1] * barycentricPosition.y + faceContext.z[2] * barycentricPosition.z;
                    
                    //像素着色器
                    shader.PixelShading(pixelInContext, pixelOutContext);

                    configuration.colorBuffer->SetData(pixelOutContext.color, screenPosition.x, screenPosition.y);
                }
            }
        }
    }
}
void MeshRenderer::LogMatrix(glm::mat4 matrix)
{
    qDebug() << "Matrix :";
    for (int i = 0; i < 4; i++)
    {
        QString s = "";
        for (int j = 0; j < 4; j++)
        {
            s += QString::number(matrix[j][i], 'f', 6);
            s += " ";
        }
        qDebug() << s;
    }
}
