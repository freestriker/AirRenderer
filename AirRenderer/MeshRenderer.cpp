#include "MeshRenderer.h"
#include <QDebug>
#include <qdir.h>
#include <QCoreApplication>
#include "Drawer.h"
MeshRenderer::MeshRenderer()
{
	std::string s = (QCoreApplication::applicationDirPath() + '/' + "../../Model/Triangle.obj").toStdString();
	qDebug() << "Current working directory: " << QCoreApplication::applicationDirPath() << endl;
	if (!OpenMesh::IO::read_mesh(mesh, s))
	{
		qDebug() << "Failed to load " << QString::fromStdString(s) << endl;
		exit(1);
	}
}

void MeshRenderer::Render(glm::mat4 mvpMatrix, glm::mat4 screenMatrix)
{
    std::vector<VertexContext> vertexContext = std::vector<VertexContext>(mesh.vertices_end().handle().idx(), VertexContext());
    for (Mesh::VertexIter v_it = mesh.vertices_sbegin(); v_it != mesh.vertices_end(); v_it++)
    {
        //顶点着色器
        Mesh::Point p = mesh.point(*v_it);
        vertexContext[v_it.handle().idx()].position = mvpMatrix * glm::vec4(p[0], p[1], p[2], 1);
        qDebug() << v_it.handle().idx() << endl;
    }
    for (Mesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
    {
        FaceContext faceContext = FaceContext();
        int vertexIndex = 0, inBoundryCount = 0;
        for (Mesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
        {
            glm::vec4 pos = vertexContext[fv_it.handle().idx()].position;

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

            faceContext.position[vertexIndex] = ivec2(pos.x, pos.y);
            faceContext.z[vertexIndex] = pos.z;
            faceContext.w[vertexIndex] = pos.w;
            vertexIndex++;
        }
        if (inBoundryCount > 0)
        {
            //像素着色器
            Drawer::DrawTriangle_CheckBoundry(faceContext.position[0], faceContext.position[1], faceContext.position[2], Color::white, *configuration.colorBuffer, configuration.resolution.width, configuration.resolution.height);
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
