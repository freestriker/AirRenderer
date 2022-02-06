#include "MeshRenderer.h"
#include <QDebug>
#include <qdir.h>
#include <QCoreApplication>
#include "Drawer.h"
MeshRenderer::MeshRenderer()
{
	std::string s = (QCoreApplication::applicationDirPath() + '/' + "../../Model/Flat.obj").toStdString();
	qDebug() << "Current working directory: " << QCoreApplication::applicationDirPath() << endl;
	if (!OpenMesh::IO::read_mesh(mesh, s))
	{
		qDebug() << "Failed to load " << QString::fromStdString(s) << endl;
		exit(1);
	}
}

void MeshRenderer::Render(glm::mat4 mvpMatrix, glm::mat4 screenMatrix)
{
    for (Mesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
    {
        std::vector<glm::vec4> vertexes = std::vector<glm::vec4>();
        std::vector<float> ws = std::vector<float>();
        int inCount = 0;
        for (Mesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
        {
            //±‰ªª
            Mesh::Point p = mesh.point(*fv_it);
            glm::vec4 pos = glm::vec4(p[0], p[1], p[2], 1);
            pos = mvpMatrix * pos;

            //≤‚ ‘Ãﬁ≥˝
            if (-pos.w < pos.x && pos.x < pos.w
                && -pos.w < pos.y && pos.y < pos.w
                && 0 < pos.z && pos.z < pos.w)
            {
                inCount++;
            }

            //Õ∏ ”≥˝∑®
            float w = pos.w;
            ws.push_back(w);
            pos = pos / w;

            vertexes.push_back(pos);
        }

        //Ãﬁ≥˝
        if (inCount == 0)
        {

        }
        else
        {
            LogMatrix(screenMatrix);
            std::vector<ivec2> pos = std::vector<ivec2>();
            for (int i = 0; i < 3; i++)
            {
                vertexes[i] = screenMatrix * vertexes[i];
                pos.push_back(ivec2(vertexes[i].x, vertexes[i].y));
            }
            Drawer::DrawTriangle_CheckBoundry(pos[0], pos[1], pos[2], Color::white, *configuration.colorBuffer, configuration.resolution.width, configuration.resolution.height);
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
