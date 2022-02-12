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


MeshRenderer::MeshRenderer():MeshRenderer("../../Model/Flat_Wall_Normal.ply")
{

}
MeshRenderer::MeshRenderer(std::string filePath):Component("MeshRenderer")
{
	std::string s = (QCoreApplication::applicationDirPath() + "/" + QString::fromStdString(filePath)).toStdString();
    OpenMesh::IO::Options o = OpenMesh::IO::Options::ColorFloat + OpenMesh::IO::Options::VertexColor + OpenMesh::IO::Options::ColorAlpha + OpenMesh::IO::Options::VertexTexCoord;
    mesh.request_vertex_colors();
    mesh.request_vertex_texcoords2D();
	if (!OpenMesh::IO::read_mesh(mesh, s, o, true))
	{
		qDebug() << "Failed to load " << QString::fromStdString(s) << endl;
		exit(1);
	}
    shader = Shader();
    material = Material();
}

void MeshRenderer::Render(glm::mat4 mvpMatrix, glm::mat4 screenMatrix)
{
    std::vector<VertexOutContext> vertexOutContext = std::vector<VertexOutContext>(mesh.vertices_end().handle().idx(), VertexOutContext());
    for (Mesh::VertexIter v_it = mesh.vertices_sbegin(); v_it != mesh.vertices_end(); v_it++)
    {
        int index = (*v_it).idx();
        Mesh::Point p = mesh.point(*v_it);
        Mesh::TexCoord2D uv = mesh.texcoord2D(v_it);
        VertexInContext vertexInContext = VertexInContext();
        vertexInContext.position = glm::vec4(p[0], p[1], p[2], 1);
        vertexInContext.texcoord1 = glm::vec2(uv[0], uv[1]);
        vertexInContext.color = Color::white;
        vertexInContext.mvpMatrix = mvpMatrix;
        vertexInContext.vertexIndex = index;

        //顶点着色器
        shader.VertexShading(vertexInContext, vertexOutContext[index], material);
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
        //if (inBoundryCount > 0)
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
                    pixelInContext.texcoord1 = vertexOutContext[faceContext.vertexIndex[0]].texcoord1 * float(barycentricPosition.x) + vertexOutContext[faceContext.vertexIndex[1]].texcoord1 * float(barycentricPosition.y) + vertexOutContext[faceContext.vertexIndex[2]].texcoord1 * float(barycentricPosition.z);
                    pixelInContext.w = faceContext.w[0] * barycentricPosition.x + faceContext.w[1] * barycentricPosition.y + faceContext.w[2] * barycentricPosition.z;
                    pixelInContext.z = faceContext.z[0] * barycentricPosition.x + faceContext.z[1] * barycentricPosition.y + faceContext.z[2] * barycentricPosition.z;
                    
                    if (pixelInContext.z < configuration.depthBuffer->GetData(screenPosition.x, screenPosition.y))
                    {
                        configuration.depthBuffer->SetData(pixelInContext.z, screenPosition.x, screenPosition.y);

                        //像素着色器
                        shader.PixelShading(pixelInContext, pixelOutContext, material);

                        configuration.colorBuffer->SetData(pixelOutContext.color, screenPosition.x, screenPosition.y);
                    }
                }
            }
        }
    }
}