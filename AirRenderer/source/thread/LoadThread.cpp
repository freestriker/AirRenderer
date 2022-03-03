#include "include/thread/LoadThread.h"
#include <OpenMesh/Core/IO/Options.hh>
#include <QCoreApplication>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "include/core_object/Mesh.h"
#include "include/utils/Buffer.h"
#include "include/utils/Color.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <FreeImage.h>

LoadThread::LoadThread(QObject* parent) : QThread(parent)
{
	this->command = std::vector<LoadCommand>();
	this->resourecMap = std::map<std::string, ResourceWrap*>();
}

LoadThread::LoadCommand LoadThread::Load(std::string path, ProcessOptions processOption)
{

	commandQueueMutex.lock();

	LoadCommand loadCommand = LoadCommand();
	loadCommand.path = path;
	loadCommand.processOption = processOption;
	loadCommand.loadOption = LoadOption::LOAD;
	command.push_back(loadCommand);

	commandQueueMutex.unlock();

	commandAvailable.wakeAll();

	return loadCommand;
}
LoadThread::LoadCommand LoadThread::Load(LoadCommand& loadCommand)
{

	commandQueueMutex.lock();

	command.push_back(loadCommand);

	commandQueueMutex.unlock();

	commandAvailable.wakeAll();

	return loadCommand;
}
void LoadThread::Unload(LoadCommand& loadCommand)
{

	commandQueueMutex.lock();
	loadCommand.loadOption = LoadOption::UNLOAD;
	command.push_back(loadCommand);
	commandQueueMutex.unlock();

	commandAvailable.wakeAll();
}
void LoadThread::run()
{
	commandQueueMutex.lock();
	while(true)
	{
		if (command.size() > 0)
		{
			resourecMapMutex.lock();
			for each (LoadCommand loadCommand in command)
			{
				switch (loadCommand.loadOption)
				{
				case LoadOption::LOAD:
					if (resourecMap.count(loadCommand.path))
					{
						resourecMap[loadCommand.path]->referenceCount++;
					}
					else
					{
						ResourceWrap* resourceWrap = nullptr;
						switch (loadCommand.processOption)
						{
						case ProcessOptions::MESH:
							resourceWrap = LoadMesh(loadCommand.path);
							break;
						case ProcessOptions::TEXTURE:
							resourceWrap = LoadTexture(loadCommand.path);
							break;
						}
						resourecMap[loadCommand.path] = resourceWrap;
					}

					break;
				case LoadOption::UNLOAD:
					if (resourecMap.count(loadCommand.path))
					{
						resourecMap[loadCommand.path]->referenceCount--;
						if (resourecMap[loadCommand.path]->referenceCount == 0)
						{
							ResourceWrap* resourceWrap = resourecMap[loadCommand.path];
							resourecMap.erase(loadCommand.path);
							delete resourceWrap;
						}
					}
					break;
				}
			}
			resourecMapMutex.unlock();
			resourceAvailable.wakeAll();

			command.clear();
		}
		else
		{
			commandAvailable.wakeAll();
			resourceAvailable.wakeAll();
			commandAvailable.wait(&commandQueueMutex);
		}
	}
}
LoadThread::ResourceWrap* LoadThread::LoadTexture(std::string path)
{
	FreeImage_Initialise(TRUE);

	const char* filePath = path.c_str();
	FIBITMAP* texture = nullptr;
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filePath);
	if (fif == FREE_IMAGE_FORMAT::FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filePath);

	if ((fif != FREE_IMAGE_FORMAT::FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
	{
		texture = FreeImage_Load(fif, filePath);
	}
	else
	{
		throw "Can not read texture " + path + ".";
	}
	if (texture == nullptr)
	{
		throw "Read texture " + path + " failed.";
	}
	LoadThread::ResourceWrap* resourceWrap = new LoadThread::ResourceWrap();
	resourceWrap->referenceCount = 1;
	resourceWrap->resource = texture;
	return resourceWrap;
}

LoadThread::ResourceWrap* LoadThread::LoadMesh(std::string path)
{
	MeshWrap* wrap = new MeshWrap();
	OrientedBoundingBox* box = new OrientedBoundingBox();
	ModelMesh* mesh = new ModelMesh();

	wrap->modelMesh = mesh;
	wrap->orientedBoundingBox = box;

	LoadThread::ResourceWrap* resourceWrap = new LoadThread::ResourceWrap();
	resourceWrap->referenceCount = 1;
	resourceWrap->resource = wrap;

	std::string s = (QCoreApplication::applicationDirPath() + "/" + QString::fromStdString(path)).toStdString();
	OpenMesh::IO::Options o = OpenMesh::IO::Options::ColorFloat + OpenMesh::IO::Options::VertexColor + OpenMesh::IO::Options::ColorAlpha + OpenMesh::IO::Options::VertexTexCoord + OpenMesh::IO::Options::VertexNormal;
	mesh->request_vertex_colors();
	mesh->request_vertex_texcoords2D();
	mesh->request_vertex_normals();
	if (!OpenMesh::IO::read_mesh(*mesh, s, o, true))
	{

		throw "Can not read mesh file: " + s + ".";
	}
	std::vector<glm::vec3> faceTangent = std::vector<glm::vec3>(mesh->faces_end().handle().idx(), glm::vec3(0, 0, 0));
    for (ModelMesh::FaceIter f_it = mesh->faces_sbegin(); f_it != mesh->faces_end(); ++f_it)
    {
        int ii = 0;
        glm::vec3 pos[3] = { glm::vec3(0) };
        glm::vec2 uv[3] = { glm::vec2(0) };
        for (ModelMesh::FaceVertexIter fv_it = mesh->fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
        {
			ModelMesh::Point p = mesh->point(fv_it);
			ModelMesh::TexCoord2D tex = mesh->texcoord2D(fv_it);
            pos[ii] = glm::vec3(p[0], p[1], p[2]);
            uv[ii] = glm::vec2(tex[0], tex[1]);
            ii++;
        }
        float s1 = uv[1].x - uv[0].x;
        float t1 = uv[1].y - uv[0].y;
        float s2 = uv[2].x - uv[0].x;
        float t2 = uv[2].y - uv[0].y;
        float r = 1.0 / (s1 * t2 - s2 * t1);
        glm::vec3 tangent = t2 * (pos[1] - pos[0]) - t1 * (pos[2] - pos[0]);
        tangent = tangent * r;
        tangent = glm::normalize(tangent);

		faceTangent[f_it.handle().idx()] = tangent;
    }
	for (ModelMesh::VertexIter v_it = mesh->vertices_sbegin(); v_it != mesh->vertices_end(); ++v_it)
	{
		glm::vec3 tangent = glm::vec3(0, 0, 0);
		int i = 0;
		for (ModelMesh::VertexFaceIter vf_it = mesh->vf_iter(*v_it); vf_it.is_valid(); ++vf_it)
		{
			if (i == 0)
			{
				tangent = faceTangent[vf_it.handle().idx()];
			}
			i++;
			tangent = glm::normalize(tangent + faceTangent[vf_it.handle().idx()]);
		}

		mesh->data(v_it).tangent = ModelMesh::Point(tangent.x, tangent.y, tangent.z);
	}

	box->BuildBoundingBox(*mesh);
	return resourceWrap;
}
