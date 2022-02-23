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
    this->commandBufferList = std::vector< RenderCommandBuffer*>();
    
    //timer = new QTimer(this);
    GameObject* camera = new GameObject("Camera");
    configuration.sceneObject.AddChild(camera);
    camera->AddComponent(new PerspectiveCamera());
    //camera->AddComponent(new OrthographicCamera());
    //camera->transform.SetTranslation(glm::vec3(0, 0, 5));
    camera->transform.SetTranslationRotationScale(glm::vec3(0, 0, 5), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(1, 1, 1));

    GameObject* lightRoot = new GameObject("LightRoot");
    configuration.sceneObject.AddChild(lightRoot);

    GameObject* directionalLight1 = new GameObject("DirectionalLight1");
    lightRoot->AddChild(directionalLight1);
    directionalLight1->AddComponent(new DirectionalLight());
    //directionalLight->transform.SetRotation(glm::qua(glm::vec3(0, 0, 0)));

    //GameObject* directionalLight2 = new GameObject("DirectionalLight2");
    //lightRoot->AddChild(directionalLight2);
    //directionalLight2->AddComponent(new DirectionalLight());
    //directionalLight2->FindComponent<Light>("Light")->baseColor = Color(0, 0.7, 0.2, 1);
    ////directionalLight->transform.SetRotation(glm::qua(glm::vec3(0, 0, 0)));
    //directionalLight2->transform.SetRotation(glm::qua(glm::vec3(1.0707963267948966, 0, 0)));

    GameObject* pointLight1 = new GameObject("PointLight1");
    lightRoot->AddChild(pointLight1);
    pointLight1->AddComponent(new PointLight());
    pointLight1->FindComponent< Light>("Light")->baseColor = Color(1, 0, 0, 1);
    pointLight1->transform.SetTranslationRotationScale(glm::vec3(4, -3, 3), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(1, 1, 1));

    GameObject* pointLight2 = new GameObject("PointLight2");
    lightRoot->AddChild(pointLight2);
    pointLight2->AddComponent(new PointLight());
    pointLight2->FindComponent< Light>("Light")->baseColor = Color(0, 0, 1, 1);
    pointLight2->transform.SetTranslationRotationScale(glm::vec3(-4, -3, 3), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(1, 1, 1));

    GameObject* pointLight3 = new GameObject("PointLight3");
    lightRoot->AddChild(pointLight3);
    pointLight3->AddComponent(new PointLight());
    pointLight3->FindComponent< Light>("Light")->baseColor = Color(0, 1, 0, 1);
    pointLight3->transform.SetTranslationRotationScale(glm::vec3(0, 5, 3), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(1, 1, 1));

    GameObject* pointLight4 = new GameObject("PointLight4");
    lightRoot->AddChild(pointLight4);
    pointLight4->AddComponent(new PointLight());
    pointLight4->FindComponent< Light>("Light")->baseColor = Color(1, 1, 1, 1);
    pointLight4->transform.SetTranslationRotationScale(glm::vec3(0, 0, 8), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(1, 1, 1));


    //GameObject* go0 = new GameObject("go0");
    //configuration.sceneObject.AddChild(go0);
    //go0->transform.SetTranslationRotationScale(glm::vec3(0, 0, 0), glm::quat(glm::vec3(0.7853981633974483, 0.7853981633974483, 0.7853981633974483)), glm::vec3(2, 2, 2));
    //go0->AddComponent(new MeshRenderer("../../Model/Cube_Wall_Normal.ply"));

    GameObject* go0 = new GameObject("go0");
    configuration.sceneObject.AddChild(go0);
    go0->transform.SetTranslationRotationScale(glm::vec3(0, 2, 0), glm::quat(glm::vec3(1.5707963267948966, 0, 0)), glm::vec3(5, 5, 5));
    //go0->AddComponent(new MeshRenderer("../../Resources/Model/Sphere_Wall_Normal.ply"));
    //GameObject* go01 = new GameObject("go01");
    //configuration.sceneObject.AddChild(go01);
    //go01->transform.SetTranslationRotationScale(glm::vec3(5, 0, 0), glm::quat(glm::vec3(1.5707963267948966, 0, 0)), glm::vec3(5, 5, 5));
    //go01->AddComponent(new MeshRenderer("../../Resources/Model/Sphere_Wall_Normal.ply"));
    //GameObject* go02 = new GameObject("go02");
    //configuration.sceneObject.AddChild(go02);
    //go02->transform.SetTranslationRotationScale(glm::vec3(-5, 0, 0), glm::quat(glm::vec3(1.5707963267948966, 0, 0)), glm::vec3(5, 5, 5));
    //go02->AddComponent(new MeshRenderer("../../Resources/Model/Sphere_Wall_Normal.ply"));

    //LoadThread::LoadCommand lc = LoadThread::LoadCommand();
    //lc.loadOption = LoadThread::LoadOption::LOAD;
    //lc.path = "../../Resources/Model/Sphere_Wall_Normal.ply";
    //lc.processOption = LoadThread::ProcessOptions::MESH;

    //global.loadThread->AddCommand(lc);
    //global.loadThread->AddCommand(lc);
    //global.loadThread->AddCommand(lc);
    //global.loadThread->AddCommand(lc);
    //global.loadThread->AddCommand(lc);
    //global.loadThread->AddCommand(lc);
    //global.loadThread->AddCommand(lc);
    //global.loadThread->AddCommand(lc);
    //global.loadThread->AddCommand(lc);
    //global.loadThread->AddCommand(lc);
    //global.loadThread->AddCommand(lc);

    GameObject* go1 = new GameObject("go1");
    configuration.sceneObject.AddChild(go1);
    go1->transform.SetTranslation(glm::vec3(5, 0, 0));

    //GameObject* go11 = new GameObject("go11");
    //go1->AddChild(go11);
    //go11->transform.SetTranslation(glm::vec3(0, 5, -1));
    //go11->AddComponent(new MeshRenderer());

    //GameObject* go12 = new GameObject("go12");
    //go1->AddChild(go12);
    //go12->transform.SetTranslation(glm::vec3(0, -5, -1));
    //go12->AddComponent(new MeshRenderer());

    GameObject* go13 = new GameObject("go13");
    go1->AddChild(go13);
    go13->transform.SetTranslation(glm::vec3(-5, 0, 0));

    //GameObject* go131 = new GameObject("go131");
    //go13->AddChild(go131);
    //go131->transform.SetTranslation(glm::vec3(0, 0, -1));
    //go131->AddComponent(new MeshRenderer());

    //GameObject* go132 = new GameObject("go132");
    //go13->AddChild(go132);
    //go132->transform.SetTranslation(glm::vec3(0.45, 0, -2));
    //go132->AddComponent(new MeshRenderer());

    //GameObject* go133 = new GameObject("go133");
    //go13->AddChild(go133);
    //go133->transform.SetTranslation(glm::vec3(6, 0, -3));
    //go133->transform.SetScale(glm::vec3(5, 5, 1));
    //go133->AddComponent(new MeshRenderer("../../Resources/Model/Flat_Wall_Normal.ply"));
    //GameObject* go134 = new GameObject("go1334");
    //go13->AddChild(go134);
    //go134->transform.SetTranslation(glm::vec3(-6, 0, -3));
    //go134->transform.SetScale(glm::vec3(5, 5, 1));
    //go134->AddComponent(new MeshRenderer("../../Resources/Model/Flat_Wall_Normal.ply"));
    GameObject* go133 = new GameObject("go133");
    go13->AddChild(go133);
    //go133->transform.SetTranslation(glm::vec3(0, 0, -3));
    //go133->transform.SetScale(glm::vec3(18, 18, 1));
    go133->transform.SetRotation(glm::vec3(0.2617993877991494, 0.2617993877991494, 0.2617993877991494));
    go133->transform.SetScale(glm::vec3(2, 2, 1));
    go133->AddComponent(new MeshRenderer("../../Resources/Model/Flat_Wall_Normal.ply"));

    //GameObject* go134 = new GameObject("go134");
    //go13->AddChild(go133);
    //go134->transform.SetTranslation(glm::vec3(0, 0, 0));
    //go134->transform.SetScale(glm::vec3(2, 2, 1));
    //go134->AddComponent(new MeshRenderer());
}
void RenderThread::SubmitCommandBuffer(RenderCommandBuffer& renderCommandBuffer)
{
    commandBufferMutex.lock();
    if (commandBufferList.size() == commandBufferList.capacity())
    {
        commandBufferList.erase(commandBufferList.begin());

    }
    commandBufferList.push_back(&renderCommandBuffer);
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
            RenderCommandBuffer* rcb = commandBufferList[0];
            commandBufferList.erase(commandBufferList.begin());
            commandBufferMutex.unlock();

            Render(*rcb);
            Display();
            sleep(0);
            commandBufferMutex.lock();
        }
        else
        {
            commandBufferAvailable.wait(&commandBufferMutex);
        }
    }
}

void RenderThread::Run()
{
    //Render();
    //timer->setInterval(900);
    //connect(timer, SIGNAL(timeout()), this, SLOT(Render()));
    //timer->start();
    //this->exec();
    RenderCommandBuffer renderCommandBuffer = RenderCommandBuffer();
    std::vector<RenderItem<GameObject>> cameras = std::vector<RenderItem<GameObject>>();
    std::vector<RenderItem<GameObject>> lights = std::vector<RenderItem<GameObject>>();
    std::vector<RenderItem<GameObject>> renderers = std::vector<RenderItem<GameObject>>();
    GetCameras(cameras);
    GetLights(lights);
    GetMeshRenderers(renderers);
    for each (RenderItem<GameObject> renderItem in lights)
    {
        renderCommandBuffer.AddLight(*renderItem.item->FindComponent<Light>("Light"));
    }
    for each (RenderItem<GameObject> renderItem in cameras)
    {
        renderCommandBuffer.SetCamera(*renderItem.item->FindComponent<Camera>("Camera"));
        for each (RenderItem<GameObject> renderItem in renderers)
        {
            MeshRenderer* mr = renderItem.item->FindComponent<MeshRenderer>("MeshRenderer");
            renderCommandBuffer.DrawMesh(mr->mesh, mr->gameObject->transform.worldMatrix, *mr->material);
        }
    }
    Render(renderCommandBuffer);
    Display();
}
void RenderThread::GetCameras(std::vector<RenderItem<GameObject>>& vector)
{
    vector.clear();
    for (GameObject::ChildIterator i = configuration.sceneObject.GetStartChildIterator(), end = configuration.sceneObject.GetEndChildIterator(); i != end; i++)
    {
        GetCamerasDFS(vector, *i);
    }

}
void RenderThread::GetCamerasDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject)
{
    for (GameObject::ChildIterator i = gameObject->GetStartChildIterator(), end = gameObject->GetEndChildIterator(); i != end; i++)
    {
        GetCamerasDFS(vector, *i);
    }
    if (gameObject->FindComponent<Camera>("Camera"))
    {
        GameObject* parent = gameObject;
        glm::mat4 m = glm::mat4(1);
        while (parent)
        {
            m = parent->transform.TranslationMatrix() * parent->transform.RotationMatrix() * m;
            parent = parent->parent;
        }
        vector.push_back(RenderItem<GameObject>(gameObject, m));
    }
}

void RenderThread::GetMeshRenderers(std::vector<RenderItem<GameObject>>& vector)
{
    vector.clear();
    for (GameObject::ChildIterator i = configuration.sceneObject.GetStartChildIterator(), end = configuration.sceneObject.GetEndChildIterator(); i != end; i++)
    {
        GetMeshRenderersDFS(vector, *i, glm::mat4(1));
    }

}
void RenderThread::GetMeshRenderersDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix)
{
    glm::mat4 matrix = parentMatrix * gameObject->transform.TranslationMatrix() * gameObject->transform.RotationMatrix() * gameObject->transform.ScaleMatrix();
    for (GameObject::ChildIterator i = gameObject->GetStartChildIterator(), end = gameObject->GetEndChildIterator(); i != end; i++)
    {    
        GetMeshRenderersDFS(vector, *i, matrix);
    }
    MeshRenderer* meshRenderer = gameObject->FindComponent<MeshRenderer>("MeshRenderer");
    if (meshRenderer)
    {
        vector.push_back(RenderItem<GameObject>(gameObject, meshRenderer->gameObject->transform.worldMatrix));
        //Utils::LogMatrix("MeshRenderer World", meshRenderer->gameObject->transform.worldMatrix);
    }
}
void RenderThread::GetLights(std::vector<RenderItem<GameObject>>& vector)
{
    vector.clear();
    for (GameObject::ChildIterator i = configuration.sceneObject.GetStartChildIterator(), end = configuration.sceneObject.GetEndChildIterator(); i != end; i++)
    {
        GetLightsDFS(vector, *i, glm::mat4(1));
    }

}
void RenderThread::GetLightsDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix)
{
    for (GameObject::ChildIterator i = gameObject->GetStartChildIterator(), end = gameObject->GetEndChildIterator(); i != end; i++)
    {    
        GetLightsDFS(vector, *i, parentMatrix);
    }
    Light* light = gameObject->FindComponent<Light>("Light");
    if (light)
    {
        vector.push_back(RenderItem<GameObject>(gameObject, parentMatrix));
    }
}

void RenderThread::Render(RenderCommandBuffer& renderCommandBuffer)
{
    LightContext lightContext = LightContext();
    lightContext.lights = renderCommandBuffer.lights;

    for (RenderCommandBuffer::CameraRenderWrap cameraRenderWrap : renderCommandBuffer.cameraRenderWrap)
    {
        CameraContext cameraContext = cameraRenderWrap.cameraContext;
        
        MatrixContext matrixContext = MatrixContext();
        matrixContext.viewMatrix = glm::inverse(cameraContext.worldMatrix);
        matrixContext.projectionMatrix = cameraContext.projectionMatrix;
        matrixContext.rasterizationMatrix = configuration.GetScreenMatrix();
        matrixContext.vpMatrix = matrixContext.projectionMatrix * matrixContext.viewMatrix;

        for (RenderCommandBuffer::MaterialRenderWrap materialRenderWrap : renderCommandBuffer.materialRenderWrap)
        {
            matrixContext.worldMatrix = materialRenderWrap.worldMatrix;
            matrixContext.wvMatrix = matrixContext.viewMatrix * matrixContext.worldMatrix;
            matrixContext.wv_tiMatrix = glm::inverse(glm::transpose(matrixContext.wvMatrix));
            matrixContext.w_tiMatrix = glm::inverse(glm::transpose(matrixContext.worldMatrix));
            matrixContext.wvpMatrix = matrixContext.vpMatrix * matrixContext.worldMatrix;

            Pipeline(&matrixContext, &lightContext, &cameraContext, &renderCommandBuffer.mesh[materialRenderWrap.meshIndex], materialRenderWrap.material->Shader());
        }
    }
}
void RenderThread::Pipeline(MatrixContext* matrixContext, LightContext* lightContext, CameraContext* cameraContext, Mesh* mesh, ShaderBase* shader)
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
    delete shader;
}


void RenderThread::Render()
{
    std::vector<RenderItem<GameObject>> cameraItems = std::vector<RenderItem<GameObject>>();
    GetCameras(cameraItems);
    std::vector<RenderItem<GameObject>> meshRendererItems = std::vector<RenderItem<GameObject>>();
    GetMeshRenderers(meshRendererItems);
    std::vector<RenderItem<GameObject>> lightItems = std::vector<RenderItem<GameObject>>();
    GetLights(lightItems);

    LightContext lightContext = LightContext();
    lightContext.lights = std::vector<Light*>();
    for (RenderItem<GameObject> lightItem : lightItems)
    {
        lightContext.lights.push_back(lightItem.item->FindComponent<Light>("Light"));
    }

    for (RenderItem<GameObject> cameraItem : cameraItems)
    {
        CameraContext cameraContext = CameraContext();
        cameraContext.needPerspectiveCorrection = cameraItem.item->FindComponent<Camera>("Camera")->needPerspectiveCorrection;
        
        MatrixContext matrixContext = MatrixContext();
        matrixContext.viewMatrix = glm::inverse(cameraItem.transformationMatrix);
        Log::LogMatrix("viewMatrix", matrixContext.viewMatrix);
        matrixContext.projectionMatrix = cameraItem.item->FindComponent<Camera>("Camera")->ProjectionMatrix();
        Log::LogMatrix("projectionMatrix", matrixContext.projectionMatrix);
        matrixContext.rasterizationMatrix = configuration.GetScreenMatrix();
        matrixContext.vpMatrix = matrixContext.projectionMatrix * matrixContext.viewMatrix;

        for (RenderItem<GameObject> meshRendererItem : meshRendererItems)
        {
            matrixContext.worldMatrix = meshRendererItem.transformationMatrix;
            matrixContext.wvMatrix = matrixContext.viewMatrix * matrixContext.worldMatrix;
            matrixContext.wv_tiMatrix = glm::inverse(glm::transpose(matrixContext.wvMatrix));
            matrixContext.w_tiMatrix = glm::inverse(glm::transpose(matrixContext.worldMatrix));
            matrixContext.wvpMatrix = matrixContext.vpMatrix * matrixContext.worldMatrix;

            meshRendererItem.item->FindComponent<MeshRenderer>("MeshRenderer")->Render(&matrixContext, &lightContext, &cameraContext);
        }
    }
    Display();
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