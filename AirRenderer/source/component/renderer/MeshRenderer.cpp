#include "include/component/renderer/MeshRenderer.h"
#include "include/core_object/Configuration.h"
#include <QDebug>
#include <qdir.h>
#include <QCoreApplication>
#include "include/context/FaceContext.h"
#include "include/context/PixelInContext.h"
#include "include/context/PixelOutContext.h"
#include "include/context/PixelIterator.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "include/context/VertexInContext.h"
#include "include/context/VertexOutContext.h"
#include "include/context/FaceContext.h"


MeshRenderer::MeshRenderer():MeshRenderer("../../Resources/Model/Flat_Wall_Normal.ply")
{

}
MeshRenderer::MeshRenderer(std::string filePath):Component("MeshRenderer")
{
	std::string s = (QCoreApplication::applicationDirPath() + "/" + QString::fromStdString(filePath)).toStdString();
    OpenMesh::IO::Options o = OpenMesh::IO::Options::ColorFloat + OpenMesh::IO::Options::VertexColor + OpenMesh::IO::Options::ColorAlpha + OpenMesh::IO::Options::VertexTexCoord + OpenMesh::IO::Options::VertexNormal;
    mesh.request_vertex_colors();
    mesh.request_vertex_texcoords2D();
    mesh.request_vertex_normals();
	if (!OpenMesh::IO::read_mesh(mesh, s, o, true))
	{
		qDebug() << "Failed to load " << QString::fromStdString(s) << endl;
		exit(1);
	}
    shader = Shader();
    material = Material();
}

void MeshRenderer::Render(MatrixContext* matrixContext, LightContext* lightContext)
{
    FaceContext faceContext = FaceContext();
    for (Mesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
    {
        VertexOutContext vertexOutContext[3] = {VertexOutContext(), VertexOutContext(), VertexOutContext()};
        VertexInContext vertexInContext = VertexInContext();

        //切线
        int ii = 0;
        glm::vec3 pos[3] = { glm::vec3(0) };
        glm::vec2 uv[3] = { glm::vec2(0) };
        for (Mesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
        {
            Mesh::Point p = mesh.point(fv_it);
            Mesh::TexCoord2D tex = mesh.texcoord2D(fv_it);
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

        int index = 0, inBoundryCount = 0;
        for (Mesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
        {
            Mesh::Point p = mesh.point(fv_it);
            Mesh::Point vn = mesh.normal(fv_it);
            Mesh::TexCoord2D uv = mesh.texcoord2D(fv_it);
            vertexInContext.mPosition = glm::vec4(p[0], p[1], p[2], 1);
            vertexInContext.mNormal = glm::normalize(glm::vec3(vn[0], vn[1], vn[2]));
            vertexInContext.mTangent = tangent;
            vertexInContext.texcoord1 = glm::vec2(uv[0], uv[1]);
            vertexInContext.color = Color::white;
            vertexInContext.vertexIndex = index;

            //顶点着色器
            shader.VertexShading(vertexInContext, vertexOutContext[index], material, matrixContext, lightContext);

            glm::vec4 pos = vertexOutContext[index].pPosition;

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
            pos = matrixContext->rasterizationMatrix * pos;

            //图元装配
            faceContext.screenPosition[index] = glm::ivec2(pos.x + 0.5, pos.y + 0.5);
            faceContext.z[index] = pos.z;
            faceContext.w[index] = pos.w;
            faceContext.vertexIndex[index] = index;
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
                    
                    pixelInContext.pPosition = vertexOutContext[faceContext.vertexIndex[0]].pPosition * float(barycentricPosition.x) + vertexOutContext[faceContext.vertexIndex[1]].pPosition * float(barycentricPosition.y) + vertexOutContext[faceContext.vertexIndex[2]].pPosition * float(barycentricPosition.z);
                    pixelInContext.vPosition = vertexOutContext[faceContext.vertexIndex[0]].vPosition * float(barycentricPosition.x) + vertexOutContext[faceContext.vertexIndex[1]].vPosition * float(barycentricPosition.y) + vertexOutContext[faceContext.vertexIndex[2]].vPosition * float(barycentricPosition.z);
                    pixelInContext.wPosition = vertexOutContext[faceContext.vertexIndex[0]].wPosition * float(barycentricPosition.x) + vertexOutContext[faceContext.vertexIndex[1]].wPosition * float(barycentricPosition.y) + vertexOutContext[faceContext.vertexIndex[2]].wPosition * float(barycentricPosition.z);
                    pixelInContext.vNormal = glm::normalize(vertexOutContext[faceContext.vertexIndex[0]].vNormal * float(barycentricPosition.x) + vertexOutContext[faceContext.vertexIndex[1]].vNormal * float(barycentricPosition.y) + vertexOutContext[faceContext.vertexIndex[2]].vNormal * float(barycentricPosition.z));
                    pixelInContext.wNormal = glm::normalize(vertexOutContext[faceContext.vertexIndex[0]].wNormal * float(barycentricPosition.x) + vertexOutContext[faceContext.vertexIndex[1]].wNormal * float(barycentricPosition.y) + vertexOutContext[faceContext.vertexIndex[2]].wNormal * float(barycentricPosition.z));
                    pixelInContext.vTangent = vertexOutContext[0].vTangent;
                    pixelInContext.wTangent = vertexOutContext[0].wTangent;
                    pixelInContext.color = vertexOutContext[faceContext.vertexIndex[0]].color * barycentricPosition.x + vertexOutContext[faceContext.vertexIndex[1]].color * barycentricPosition.y + vertexOutContext[faceContext.vertexIndex[2]].color * barycentricPosition.z;
                    pixelInContext.texcoord1 = vertexOutContext[faceContext.vertexIndex[0]].texcoord1 * float(barycentricPosition.x) + vertexOutContext[faceContext.vertexIndex[1]].texcoord1 * float(barycentricPosition.y) + vertexOutContext[faceContext.vertexIndex[2]].texcoord1 * float(barycentricPosition.z);
                    pixelInContext.w = faceContext.w[0] * barycentricPosition.x + faceContext.w[1] * barycentricPosition.y + faceContext.w[2] * barycentricPosition.z;
                    pixelInContext.z = faceContext.z[0] * barycentricPosition.x + faceContext.z[1] * barycentricPosition.y + faceContext.z[2] * barycentricPosition.z;
                    
                    if (pixelInContext.z < configuration.depthBuffer->GetData(screenPosition.x, screenPosition.y))
                    {
                        configuration.depthBuffer->SetData(pixelInContext.z, screenPosition.x, screenPosition.y);

                        //像素着色器
                        shader.PixelShading(pixelInContext, pixelOutContext, material, matrixContext, lightContext);

                        configuration.colorBuffer->SetData(pixelOutContext.color, screenPosition.x, screenPosition.y);
                    }
                }
            }
        }
    }
}