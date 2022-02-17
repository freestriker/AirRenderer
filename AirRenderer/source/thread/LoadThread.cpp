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

void LoadThread::AddCommand(LoadThread::LoadCommand loadCommand)
{
	commandQueueMutex.lock();
	commandQueue.push(loadCommand);
	commandQueueMutex.unlock();

	commandAvailable.wakeAll();
}
LoadThread::ResourceWrap* LoadThread::GetResourceWrap(std::string path)
{
	QMutexLocker m_lock(&resourecMapMutex);
	while (!resourecMap.count(path))
	{
		resourceAvailable.wait(&resourecMapMutex);
	}
	return resourecMap[path];
}
void LoadThread::run()
{
	commandQueueMutex.lock();
	while(true)
	{
		commandAvailable.wait(&commandQueueMutex);
		while (commandQueue.size() > 0)
		{
			LoadCommand loadCommand = commandQueue.front();
			commandQueue.pop();

			resourecMapMutex.lock();
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
			resourecMapMutex.unlock();
		}
		resourceAvailable.wakeAll();
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


	int width = FreeImage_GetWidth(texture);
	int height = FreeImage_GetHeight(texture);
	int pixelCount = width * height;
	Buffer<Color>* buffer = new Buffer<Color>(width, height, Color::white);
	BYTE* pixels = (BYTE*)FreeImage_GetBits(texture);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int pixelIndex = buffer->GetIndex(i, j);
			buffer->SetData(Color(pixels[pixelIndex] / 255.0, pixels[pixelIndex + 1] / 255.0, pixels[pixelIndex + 2] / 255.0, pixels[pixelIndex + 3] / 255.0), i, j);
		}
	}
	FreeImage_Unload(texture);
	FreeImage_DeInitialise();

	LoadThread::ResourceWrap* resourceWrap = new LoadThread::ResourceWrap();
	resourceWrap->referenceCount = 1;
	resourceWrap->resource = buffer;
	return resourceWrap;
}

LoadThread::ResourceWrap* LoadThread::LoadMesh(std::string path)
{
	Mesh* mesh = new Mesh();
	LoadThread::ResourceWrap* resourceWrap = new LoadThread::ResourceWrap();
	resourceWrap->referenceCount = 1;
	resourceWrap->resource = mesh;

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
    for (Mesh::FaceIter f_it = mesh->faces_sbegin(); f_it != mesh->faces_end(); ++f_it)
    {
        int ii = 0;
        glm::vec3 pos[3] = { glm::vec3(0) };
        glm::vec2 uv[3] = { glm::vec2(0) };
        for (Mesh::FaceVertexIter fv_it = mesh->fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
        {
			Mesh::Point p = mesh->point(fv_it);
			Mesh::TexCoord2D tex = mesh->texcoord2D(fv_it);
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
	for (Mesh::VertexIter v_it = mesh->vertices_sbegin(); v_it != mesh->vertices_end(); ++v_it)
	{
		glm::vec3 tangent = glm::vec3(0, 0, 0);
		int i = 0;
		for (Mesh::VertexFaceIter vf_it = mesh->vf_iter(*v_it); vf_it.is_valid(); ++vf_it)
		{
			if (i == 0)
			{
				tangent = faceTangent[vf_it.handle().idx()];
			}
			i++;
			tangent = glm::normalize(tangent + faceTangent[vf_it.handle().idx()]);
		}

		mesh->data(v_it).tangent = Mesh::Point(tangent.x, tangent.y, tangent.z);
	}
	return resourceWrap;
}
