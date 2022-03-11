#include "include/component/renderer/MeshRenderer.h"
#include "include/core_object/Configuration.h"
#include <QDebug>
#include <qdir.h>
#include <QCoreApplication>
#include "include/context/PixelInContext.h"
#include "include/context/PixelOutContext.h"
#include "include/context/PixelIterator.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "include/context/VertexInContext.h"
#include "include/context/VertexOutContext.h"
#include <include/thread/LoadThread.h>
#include <include/core_object/Global.h>
#include <include/material/NormalMaterial.h>
#include <include/texture/Texture.h>
#include <include/utils/RegisterIndex.h>

MeshRenderer::MeshRenderer():MeshRenderer("..\\Resources\\Model\\Flat_Wall_Normal.ply")
{

}
MeshRenderer::~MeshRenderer()
{

}
MeshRenderer::MeshRenderer(std::string filePath):Component("MeshRenderer")
{
    this->mesh = Mesh(filePath);
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

void MeshRenderer::Render(RenderCommandBufferBuilder& commandBuilder)
{
    commandBuilder.DrawMesh(mesh, gameObject->transform.worldMatrix, *material, -1);
}
