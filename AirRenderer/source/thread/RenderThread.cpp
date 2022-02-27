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
#include <include/context/PrimitiveContext.h>
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
            matrixContext.wv_itMatrix = glm::transpose(glm::inverse(matrixContext.wvMatrix));
            matrixContext.w_itMatrix = glm::transpose(glm::inverse(matrixContext.worldMatrix));
            matrixContext.wvpMatrix = matrixContext.vpMatrix * matrixContext.worldMatrix;

            ShaderBase* sb = materialRenderWrap.materialInstance->Shader();
            for (int i = 0; i < MAX_SHADER_PASS_COUNT; i++)
            {
                if (sb->activeTable[i])
                {
                    Pipeline(&matrixContext, &lightContext, &cameraContext, &renderCommandBuffer->meshInstances[materialRenderWrap.meshInstanceIndex], sb->shaderPasses[i]);
                }
            }

        }
    }
}
void RenderThread::Pipeline(MatrixContext* matrixContext, LightContext* lightContext, CameraContext* cameraContext, Mesh* mesh, ShaderPass& shaderPass)
{

    ModelMesh* modelMesh = mesh->GetModelMesh();
    std::function<bool(PrimitiveContext&, glm::vec3*)> checkCull = CalulateCheckCullOption(shaderPass.shaderOption);
    std::function<bool(float, float)> checkZTest = CalulateZTestOption(shaderPass.shaderOption);
    std::function<bool()> checkZWrite = CalulateZWriteOption(shaderPass.shaderOption);
    std::function<bool()> checkAlphaTest = CalulateAlphaTestOption(shaderPass.shaderOption);
    std::function<bool()> checkEarlyZ = CalulateEarlyZ(shaderPass.shaderOption);
    std::function<bool()> checkAutoZ = CalulateAutoZ(shaderPass.shaderOption);
    std::function<Color(Color&, Color&)> alphaBlend = CalulateAlphaBlendOption(shaderPass.shaderOption);
    //顶点阶段
    std::vector<VertexOutContext> vertexOutContexts = std::vector<VertexOutContext>(modelMesh->vertices_end().handle().idx());
    VertexInContext vertexInContext = VertexInContext();
    for (ModelMesh::VertexIter v_it = modelMesh->vertices_sbegin(), v_end = modelMesh->vertices_end(); v_it != v_end; ++v_it)
    {
        ModelMesh::Point p = modelMesh->point(v_it);
        ModelMesh::Point t = modelMesh->data(v_it).tangent;
        ModelMesh::Point n = modelMesh->normal(v_it);
        ModelMesh::TexCoord2D uv = modelMesh->texcoord2D(v_it);
        ModelMesh::Color c = modelMesh->color(v_it);
        vertexInContext.data[RegisterIndex::POSITION] = glm::vec4(p[0], p[1], p[2], 1);
        vertexInContext.data[RegisterIndex::NORMAL] = glm::vec4(n[0], n[1], n[2], 0);
        vertexInContext.data[RegisterIndex::TANGENT] = glm::vec4(t[0], t[1], t[2], 0);
        vertexInContext.data[RegisterIndex::COLOR] = glm::vec4(c[0], c[1], c[2], 1);
        vertexInContext.data[RegisterIndex::TEXCOORD1] = glm::vec4(uv[0], uv[1], 0, 0);

        int vertexIndex = v_it.handle().idx();
        vertexInContext.vertexIndex = vertexIndex;

        //顶点着色器
        shaderPass.vertexShading(vertexInContext, vertexOutContexts[vertexIndex], matrixContext, lightContext);
    }

    //几何阶段
    PrimitiveContext primitiveInContext = PrimitiveContext();
    primitiveInContext.primitiveType = PrimitiveType::TRIANGLE;
    std::vector< PrimitiveContext> primitiveOutContexts = std::vector< PrimitiveContext>();
    PrimitiveOutContextBuilder primitiveOutContextBuilder = PrimitiveOutContextBuilder(vertexOutContexts, primitiveOutContexts);
    for (ModelMesh::FaceIter f_it = modelMesh->faces_begin(), f_end = modelMesh->faces_end(); f_it != f_end; ++f_it)
    {
        int index = 0;
        for (ModelMesh::FaceVertexIter fv_it = modelMesh->fv_iter(f_it); fv_it.is_valid(); ++fv_it)
        {
            int vertexIndex = fv_it.handle().idx();
            primitiveInContext.vertexIndexes[index] = vertexIndex;
            ++index;
        }
        //几何着色器
        shaderPass.geometryShading(primitiveInContext, primitiveOutContextBuilder, matrixContext, lightContext);
    }

    //光栅化阶段
    for (int i = 0, size = vertexOutContexts.size(); i < size; ++i)
    {
        VertexOutContext& vertexOutContext = vertexOutContexts[i];

        glm::vec4 pos = vertexOutContext.data[RegisterIndex::POSITION];

        ////测试剔除
        //if (-pos.w < pos.x && pos.x < pos.w
        //    && -pos.w < pos.y && pos.y < pos.w
        //    && 0 < pos.z && pos.z < pos.w)
        //{
        //    inBoundryCount++;
        //}

        //透视除法
        float w = pos.w;
        pos = pos / w;
        pos.w = 1.0;

        //光栅化
        pos = matrixContext->rasterizationMatrix * pos;

        vertexOutContext.depth = w;
        vertexOutContext.screenPosition = pos;
    }

    //像素阶段
    PixelInContext pixelInContext = PixelInContext();
    PixelOutContext pixelOutContext = PixelOutContext();
    for (int i = 0, size = primitiveOutContexts.size(); i < size; ++i)
    {
        PrimitiveContext& primitiveContext = primitiveOutContexts[i];
        glm::vec3 screenPositions[3] = { vertexOutContexts[primitiveContext.vertexIndexes[0]].screenPosition, vertexOutContexts[primitiveContext.vertexIndexes[1]].screenPosition, vertexOutContexts[primitiveContext.vertexIndexes[2]].screenPosition };
        float depths[3] = { vertexOutContexts[primitiveContext.vertexIndexes[0]].depth, vertexOutContexts[primitiveContext.vertexIndexes[1]].depth, vertexOutContexts[primitiveContext.vertexIndexes[2]].depth };
        if (!checkCull(primitiveContext, screenPositions))continue;
        for (PixelIterator pixelIterator = PixelIterator(primitiveContext, vertexOutContexts); pixelIterator.CheckValid(); pixelIterator++)
        {
            glm::ivec2 screenPosition = pixelIterator.GetScreenPosition();

            if (!(0 <= screenPosition.x && screenPosition.x < configuration.resolution.width
                && 0 <= screenPosition.y && screenPosition.y < configuration.resolution.height)) continue;

            //插值
            glm::dvec3 interpolationCoefficient = pixelIterator.GetInterpolationCoefficient(cameraContext);
            pixelInContext.screenPosition = screenPosition;
            for (int i = 0; i < 8; i++)
            {
                pixelInContext.data[i] =
                    vertexOutContexts[primitiveContext.vertexIndexes[0]].data[i] * float(interpolationCoefficient.x)
                    + vertexOutContexts[primitiveContext.vertexIndexes[1]].data[i] * float(interpolationCoefficient.y)
                    + vertexOutContexts[primitiveContext.vertexIndexes[2]].data[i] * float(interpolationCoefficient.z);
            }
            pixelInContext.depth = depths[0] * interpolationCoefficient.x + depths[1] * interpolationCoefficient.y + depths[2] * interpolationCoefficient.z;

            if (checkEarlyZ())
            {
                if (checkZTest(pixelInContext.depth, configuration.depthBuffer->GetData(screenPosition.x, screenPosition.y)))
                {
                    if (checkZWrite())
                    {
                        configuration.depthBuffer->SetData(pixelInContext.depth, screenPosition.x, screenPosition.y);
                    }
                    //像素着色器
                    shaderPass.pixelShading(pixelInContext, pixelOutContext, matrixContext, lightContext);
                    Color c = configuration.colorBuffer->GetData(screenPosition.x, screenPosition.y);
                    c = alphaBlend(pixelOutContext.color, c);
                    configuration.colorBuffer->SetData(c, screenPosition.x, screenPosition.y);
                }
            }
            else
            {
                bool clip = shaderPass.pixelShading(pixelInContext, pixelOutContext, matrixContext, lightContext);
                if (clip && checkAlphaTest())
                {
                    continue;
                }
                Color c = configuration.colorBuffer->GetData(screenPosition.x, screenPosition.y);
                c = alphaBlend(pixelOutContext.color, c);
                configuration.colorBuffer->SetData(c, screenPosition.x, screenPosition.y);
                if (checkAutoZ())
                {
                    pixelOutContext.depth = pixelInContext.depth;
                }
                if (checkZTest(pixelOutContext.depth, configuration.depthBuffer->GetData(screenPosition.x, screenPosition.y)))
                {
                    configuration.colorBuffer->SetData(pixelOutContext.color, screenPosition.x, screenPosition.y);
                    if (checkZWrite())
                    {
                        configuration.depthBuffer->SetData(pixelOutContext.depth, screenPosition.x, screenPosition.y);
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
std::function<bool(PrimitiveContext&, glm::vec3*)> RenderThread::CalulateCheckCullOption(ShaderOption shaderOption)
{
    switch (shaderOption.cullOption)
    {
        case CullOption::CULL_OFF:
        {
            return [](PrimitiveContext& primitiveContext, glm::vec3* positions)->bool
            {
                if (primitiveContext.primitiveType == PrimitiveType::TRIANGLE)
                {
                    glm::vec3 v1 = positions[1] - positions[0];
                    glm::vec3 v2 = positions[2] - positions[1];
                    glm::vec3 fn = glm::cross(v1, v2);
                    float d = glm::dot(glm::vec3(0, 0, -1), fn);
                    return d <= -0.000001 || d >= 0.000001;
                }
                else
                {
                    return true;
                }
            };
        }
        case CullOption::CULL_FRONT:
        {
            return [](PrimitiveContext& primitiveContext, glm::vec3* positions)->bool
            {
                if (primitiveContext.primitiveType == PrimitiveType::TRIANGLE)
                {
                    glm::vec3 v1 = positions[1] - positions[0];
                    glm::vec3 v2 = positions[2] - positions[1];
                    glm::vec3 fn = glm::cross(v1, v2);
                    float d = glm::dot(glm::vec3(0, 0, -1), fn);
                    return d <= -0.000001;
                }
                else
                {
                    return true;
                }
            };
        }
        case CullOption::CULL_BACK:
        {
            return [](PrimitiveContext& primitiveContext, glm::vec3* positions)->bool
            {
                if (primitiveContext.primitiveType == PrimitiveType::TRIANGLE)
                {
                    glm::vec3 v1 = positions[1] - positions[0];
                    glm::vec3 v2 = positions[2] - positions[1];
                    glm::vec3 fn = glm::cross(v1, v2);
                    float d = glm::dot(glm::vec3(0, 0, -1), fn);
                    return d >= 0.000001;
                }
                else
                {
                    return true;
                }
            };
        }
    }
}
std::function<bool(float, float)> RenderThread::CalulateZTestOption(ShaderOption shaderOption)
{
    switch (shaderOption.zTestOption)
    {
        case ZTestOption::Z_TEST_ON:
        {
            switch (shaderOption.zTestCompareOption)
            {
            case ZTestCompareOption::EQUAL:
            {
                return [](float depth, float depthInBuffer)->bool
                {
                    return depth == depthInBuffer;
                };
            }
            case ZTestCompareOption::LESS_EQUAL:
            {
                return [](float depth, float depthInBuffer)->bool
                {
                    return depth <= depthInBuffer;
                };
            }
            case ZTestCompareOption::LESS:
            {
                return [](float depth, float depthInBuffer)->bool
                {
                    return depth < depthInBuffer;
                };
            }
            case ZTestCompareOption::GREAT_EQUAL:
            {
                return [](float depth, float depthInBuffer)->bool
                {
                    return depth >= depthInBuffer;
                };
            }
            case ZTestCompareOption::GREAT:
            {
                return [](float depth, float depthInBuffer)->bool
                {
                    return depth > depthInBuffer;
                };
            }
            }
        }
        case ZTestOption::Z_TEST_OFF:
        {
            return [](float depth, float depthInBuffer)->bool
            {
                return true;
            };
        }
    }
}
std::function<bool()> RenderThread::CalulateZWriteOption(ShaderOption shaderOption)
{
    switch (shaderOption.zWriteOption)
    {
        case ZWriteOption::Z_WRITE_ON:
        {
            return []()->bool
            {
                return true;
            };
        }
        case ZWriteOption::Z_WRITE_OFF:
        {
            return []()->bool
            {
                return false;
            };
        }
    }
}
std::function<bool()> RenderThread::CalulateAlphaTestOption(ShaderOption shaderOption)
{
    switch (shaderOption.alphaTestOption)
    {
        case AlphaTestOption::ALPHA_TEST_ON:
        {
            return []()->bool
            {
                return true;
            };
        }
        case AlphaTestOption::ALPHA_TEST_OFF:
        {
            return []()->bool
            {
                return false;
            };
        }
    }
}
std::function<bool()> RenderThread::CalulateEarlyZ(ShaderOption shaderOption)
{
    if (shaderOption.zCalculateOption == ZCalculateOption::AUTO && shaderOption.alphaTestOption == AlphaTestOption::ALPHA_TEST_OFF && shaderOption.alphaBlendOption == AlphaBlendOption::OFF)
    {
        return []()->bool
        {
            return true;
        };
    }
    return []()->bool
    {
        return false;
    };
}
std::function<bool()> RenderThread::CalulateAutoZ(ShaderOption shaderOption)
{
    if (shaderOption.zCalculateOption == ZCalculateOption::AUTO)
    {
        return []()->bool
        {
            return true;
        };
    }
    return []()->bool
    {
        return false;
    };
}
std::function<Color(Color&, Color&)> RenderThread::CalulateAlphaBlendOption(ShaderOption shaderOption)
{
    switch (shaderOption.alphaBlendOption)
    {
    case AlphaBlendOption::OFF:
    {
        return [](Color& srcColor, Color& colorInBufer)->Color
        {
            return Color(srcColor.r, srcColor.g, srcColor.b, 1);
        };
    }
    case AlphaBlendOption::SRC_ALPHA_ONE_MINUS_SRC_ALPHA:
    {
        return [](Color& srcColor, Color& colorInBufer)->Color
        {
            Color c = srcColor * srcColor.a + colorInBufer * (1 - srcColor.a);
            c.a = 1;
            return c;
        };
    }
    }
}

