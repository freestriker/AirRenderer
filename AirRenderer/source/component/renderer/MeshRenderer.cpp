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
#include <include/thread/LoadThread.h>
#include <include/core_object/Global.h>
#include <include/core_object/Mesh.h>
#include <include/material/NormalMaterial.h>
#include <include/texture/Texture.h>
#include <include/utils/RegisterIndex.h>

MeshRenderer::MeshRenderer():MeshRenderer("../../Resources/Model/Flat_Wall_Normal.ply")
{

}
MeshRenderer::MeshRenderer(std::string filePath):Component("MeshRenderer")
{
    this->loadCommand = global.loadThread->Load(filePath, LoadThread::ProcessOptions::MESH);
    material = new NormalMaterial([](NormalData& normalData)->void
    {
        normalData.ambientReflectance = 0.2;
        normalData.convergenceExponent = 19;
        normalData.diffuseReflectance = 1;
        normalData.specularReflectance = 4;
        normalData.texture = Texture("..\\Resources\\Texture\\Wall.png", Texture::InterpolationOption::BILINERA, Texture::WrapOption::CLAMP, Texture::MipMapOption::NONE);
        normalData.normalMap = Texture("..\\Resources\\Texture\\Wall_Normal.png", Texture::InterpolationOption::BILINERA, Texture::WrapOption::CLAMP, Texture::MipMapOption::NONE);
    });
}

void MeshRenderer::Render(MatrixContext* matrixContext, LightContext* lightContext)
{
    ShaderBase* shader = material->Shader();
    
    Mesh* mesh = global.loadThread->GetResource<Mesh>(loadCommand);
    FaceContext faceContext = FaceContext();

    for (Mesh::FaceIter f_it = mesh->faces_begin(); f_it != mesh->faces_end(); ++f_it)
    {
        VertexOutContext vertexOutContext[3] = { VertexOutContext(), VertexOutContext(), VertexOutContext() };
        VertexInContext vertexInContext = VertexInContext();

        int index = 0, inBoundryCount = 0;
        for (Mesh::FaceVertexIter fv_it = mesh->fv_iter(f_it); fv_it.is_valid(); ++fv_it)
        {
            Mesh::Point p = mesh->point(fv_it);
            Mesh::Point t = mesh->data(fv_it).tangent;
            Mesh::Point n = mesh->normal(fv_it);
            Mesh::TexCoord2D uv = mesh->texcoord2D(fv_it); 
            Mesh::Color c = mesh->color(fv_it);
            vertexInContext.data[RegisterIndex::POSITION] = glm::vec4(p[0], p[1], p[2], 1);
            vertexInContext.data[RegisterIndex::NORMAL] = glm::vec4(n[0], n[1], n[2], 0);
            vertexInContext.data[RegisterIndex::TANGENT] = glm::vec4(t[0], t[1], t[2], 0);
            vertexInContext.data[RegisterIndex::COLOR] = glm::vec4(c[0], c[1], c[2], 1);
            vertexInContext.data[RegisterIndex::TEXCOORD1] = glm::vec4(uv[0], uv[1], 0, 0);

            vertexInContext.vertexIndex = fv_it.handle().idx();

            //顶点着色器
            shader->VertexShading(vertexInContext, vertexOutContext[index], matrixContext, lightContext);

            glm::vec4 pos = vertexOutContext[index].data[RegisterIndex::POSITION];

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

                    for (int i = 0; i < 8; i++)
                    {
                        pixelInContext.data[i] =
                            vertexOutContext[0].data[i] * float(barycentricPosition.x)
                            + vertexOutContext[1].data[i] * float(barycentricPosition.y)
                            + vertexOutContext[2].data[i] * float(barycentricPosition.z);
                    }
                    pixelInContext.w = faceContext.w[0] * barycentricPosition.x + faceContext.w[1] * barycentricPosition.y + faceContext.w[2] * barycentricPosition.z;
                    pixelInContext.z = faceContext.z[0] * barycentricPosition.x + faceContext.z[1] * barycentricPosition.y + faceContext.z[2] * barycentricPosition.z;

                    if (pixelInContext.z < configuration.depthBuffer->GetData(screenPosition.x, screenPosition.y))
                    {
                        configuration.depthBuffer->SetData(pixelInContext.z, screenPosition.x, screenPosition.y);

                        //像素着色器
                        shader->PixelShading(pixelInContext, pixelOutContext, matrixContext, lightContext);

                        configuration.colorBuffer->SetData(pixelOutContext.color, screenPosition.x, screenPosition.y);
                    }
                }
            }
        }
    }
    delete shader;
}
