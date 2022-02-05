#include "MeshRenderer.h"
#include <QDebug>
#include <qdir.h>
#include <QCoreApplication>
MeshRenderer::MeshRenderer()
{
	std::string s = (QCoreApplication::applicationDirPath() + '/' + "../../Model/Cube.obj").toStdString();
	qDebug() << "Current working directory: " << QCoreApplication::applicationDirPath() << endl;
	if (!OpenMesh::IO::read_mesh(mesh, s))
	{
		qDebug() << "Failed to load " << QString::fromStdString(s) << endl;
		exit(1);
	}
}
