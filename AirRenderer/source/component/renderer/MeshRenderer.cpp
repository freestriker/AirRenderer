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
#include <include/material/Material.h>
#include <include/shader/NormalData.h>
#include <include/shader/NormalShader.h>
#include <include/texture/Texture.h>
#include <include/utils/RegisterIndex.h>

MeshRenderer::MeshRenderer() :Component("MeshRenderer")/*:MeshRenderer("..\\Resources\\Model\\Flat_Wall_Normal.ply")*/
{
    material = nullptr;
    mesh = nullptr;
}
MeshRenderer::~MeshRenderer()
{
    delete material;
    delete mesh;
    typeName = "DestoriedMeshRenderer";
}
MeshRenderer::MeshRenderer(std::string filePath):Component("MeshRenderer")
{
    mesh = new Mesh(filePath);
    material = new Material(new NormalData(), new NormalShader());
}

void MeshRenderer::Render(RenderCommandBufferBuilder& commandBuilder)
{
    commandBuilder.DrawMesh(*mesh, gameObject->transform.worldMatrix, *material, -1);
}
