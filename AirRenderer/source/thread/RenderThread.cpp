#include "include/thread/RenderThread.h"
#include <vector>
#include "include/utils/Log.h"
#include "include/context/MatrixContext.h"
#include "include/component/light/Light.h"
#include "include/context/PixelIterator.h"
#include "include/component/light/DirectionalLight.h"
#include "include/component/light/PointLight.h"
#include <include/thread/LoadThread.h>
#include <include/core_object/Global.h>
#include <include/utils/Log.h>
#include <include/component/camera/OrthographicCamera.h>
#include <include/component/camera/PerspectiveCamera.h>
#include <include/context/CameraContext.h>
RenderThread::RenderThread(QObject* parent) :QThread(parent)
{
    this->commandBufferList = std::vector<std::shared_ptr<RenderCommandBuffer>>(8);
    commandBufferList.clear();
    
}
void RenderThread::SubmitCommandBuffer(std::shared_ptr<RenderCommandBuffer> renderCommandBuffer)
{
    commandBufferMutex.lock();
    if (commandBufferList.size() == commandBufferList.capacity())
    {
        commandBufferList.erase(commandBufferList.begin());

    }
    commandBufferList.push_back(renderCommandBuffer);
    commandBufferMutex.unlock();
    commandBufferAvailable.wakeAll();
}

void RenderThread::run()
{
    commandBufferMutex.lock();
    while (true)
    {
        if (!commandBufferList.empty())
        {
            std::shared_ptr<RenderCommandBuffer> rcb = commandBufferList[0];
            commandBufferList.erase(commandBufferList.begin());
            commandBufferMutex.unlock();
            qDebug() << "start render";
            Render(rcb);
            Display();
            qDebug() << "finish render";
            sleep(0);
            commandBufferMutex.lock();
        }
        else
        {
            commandBufferAvailable.wait(&commandBufferMutex);
        }
    }
}


void RenderThread::Render(std::shared_ptr<RenderCommandBuffer> renderCommandBuffer)
{
    LightContext lightContext = LightContext();
    lightContext.lights = renderCommandBuffer->lightInstances;

    for (RenderCommandBuffer::CameraRenderWrap cameraRenderWrap : renderCommandBuffer->cameraRenderWraps)
    {
        CameraContext cameraContext = cameraRenderWrap.cameraContext;
        
        MatrixContext matrixContext = MatrixContext();
        matrixContext.viewMatrix = glm::inverse(cameraContext.worldMatrix);
        matrixContext.projectionMatrix = cameraContext.projectionMatrix;
        matrixContext.rasterizationMatrix = configuration.GetScreenMatrix();
        matrixContext.vpMatrix = matrixContext.projectionMatrix * matrixContext.viewMatrix;

        for (RenderCommandBuffer::MaterialRenderWrap materialRenderWrap : renderCommandBuffer->materialRenderWraps)
        {
            matrixContext.worldMatrix = materialRenderWrap.worldMatrix;
            matrixContext.wvMatrix = matrixContext.viewMatrix * matrixContext.worldMatrix;
            matrixContext.wv_tiMatrix = glm::inverse(glm::transpose(matrixContext.wvMatrix));
            matrixContext.w_tiMatrix = glm::inverse(glm::transpose(matrixContext.worldMatrix));
            matrixContext.wvpMatrix = matrixContext.vpMatrix * matrixContext.worldMatrix;

            Pipeline(&matrixContext, &lightContext, &cameraContext, &renderCommandBuffer->meshInstances[materialRenderWrap.meshInstanceIndex], std::shared_ptr<ShaderBase>(materialRenderWrap.materialInstance->Shader()));

        }
    }
}
void RenderThread::Pipeline(MatrixContext* matrixContext, LightContext* lightContext, CameraContext* cameraContext, Mesh* mesh, std::shared_ptr<ShaderBase> shader)
{

    ModelMesh* modelMesh = mesh->GetModelMesh();
    FaceContext faceContext = FaceContext();

    for (ModelMesh::FaceIter f_it = modelMesh->faces_begin(); f_it != modelMesh->faces_end(); ++f_it)
    {
        VertexOutContext vertexOutContext[3] = { VertexOutContext(), VertexOutContext(), VertexOutContext() };
        VertexInContext vertexInContext = VertexInContext();

        int index = 0, inBoundryCount = 0;
        for (ModelMesh::FaceVertexIter fv_it = modelMesh->fv_iter(f_it); fv_it.is_valid(); ++fv_it)
        {
            ModelMesh::Point p = modelMesh->point(fv_it);
            ModelMesh::Point t = modelMesh->data(fv_it).tangent;
            ModelMesh::Point n = modelMesh->normal(fv_it);
            ModelMesh::TexCoord2D uv = modelMesh->texcoord2D(fv_it);
            ModelMesh::Color c = modelMesh->color(fv_it);
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
            //faceContext.z[index] = pos.z;
            faceContext.w[index] = w;
            faceContext.vertexIndex[index] = index;
            index++;
        }
        glm::vec3 v1 = glm::vec3(vertexOutContext[1].data[0] - vertexOutContext[0].data[0]);
        glm::vec3 v2 = glm::vec3(vertexOutContext[2].data[0] - vertexOutContext[1].data[0]);
        glm::vec3 fn = glm::cross(v1, v2);
        float d = glm::dot(glm::vec3(0, 0, 1), fn);
        if(d > 0.0001)
        //if (inBoundryCount > 0)
        {
            for (PixelIterator pixelIterator = PixelIterator(faceContext); pixelIterator.CheckValid(); pixelIterator++)
            {
                glm::ivec2 screenPosition = pixelIterator.GetScreenPosition();

                if (0 <= screenPosition.x && screenPosition.x < configuration.resolution.width
                    && 0 <= screenPosition.y && screenPosition.y < configuration.resolution.height)
                {
                    glm::dvec3 interpolationCoefficient = pixelIterator.GetInterpolationCoefficient(cameraContext);
                    PixelInContext pixelInContext = PixelInContext();
                    PixelOutContext pixelOutContext = PixelOutContext();
                    pixelInContext.screenPosition = screenPosition;

                    for (int i = 0; i < 8; i++)
                    {
                        pixelInContext.data[i] =
                            vertexOutContext[0].data[i] * float(interpolationCoefficient.x)
                            + vertexOutContext[1].data[i] * float(interpolationCoefficient.y)
                            + vertexOutContext[2].data[i] * float(interpolationCoefficient.z);
                        if (isnan(pixelInContext.data[i]).x)
                        {
                            int m = 10;
                            m += 5;
                        }
                    }
                    pixelInContext.w = faceContext.w[0] * interpolationCoefficient.x + faceContext.w[1] * interpolationCoefficient.y + faceContext.w[2] * interpolationCoefficient.z;

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
}

void RenderThread::Display()
{
    for (int x = 0; x < configuration.resolution.width; x++)
    {
        for (int y = 0; y < configuration.resolution.height; y++)
        {
            Color color = configuration.colorBuffer->GetData(x, y);
            configuration.canvas.setPixelColor(x, y, QColor(color.r * color.a * 255, color.g * color.a * 255, color.b * color.a * 255));
        }
    }
    configuration.label->setPixmap(QPixmap::fromImage(configuration.canvas));
    configuration.canvas.save("C:\\Users\\23174\\Desktop\\Out.png", "PNG", 100);

}