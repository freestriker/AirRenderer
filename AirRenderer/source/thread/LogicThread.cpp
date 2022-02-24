#include <include/thread/LogicThread.h>
#include "include/component/light/DirectionalLight.h"
#include "include/component/light/PointLight.h"
#include <include/thread/LoadThread.h>
#include <include/core_object/Global.h>
#include <include/utils/Log.h>
#include <include/component/camera/OrthographicCamera.h>
#include <include/component/camera/PerspectiveCamera.h>
#include <include/context/CameraContext.h>
#include <include/core_object/Configuration.h>
#include <include/thread/RenderCommandBuffer.h>
#include <include/thread/RenderThread.h>
void LogicThread::run()
{
    while (true)
    {
        RenderCommandBuffer* renderCommandBuffer = new RenderCommandBuffer();
        std::vector<RenderItem<GameObject>> cameras = std::vector<RenderItem<GameObject>>();
        std::vector<RenderItem<GameObject>> lights = std::vector<RenderItem<GameObject>>();
        std::vector<RenderItem<GameObject>> renderers = std::vector<RenderItem<GameObject>>();
        GetCameras(cameras);
        GetLights(lights);
        GetMeshRenderers(renderers);
        for each (RenderItem<GameObject> renderItem in lights)
        {
            renderCommandBuffer->AddLight(*renderItem.item->FindComponent<Light>("Light"));
        }
        for each (RenderItem<GameObject> renderItem in cameras)
        {
            renderCommandBuffer->SetCamera(*renderItem.item->FindComponent<Camera>("Camera"));
            for each (RenderItem<GameObject> renderItem in renderers)
            {
                MeshRenderer* mr = renderItem.item->FindComponent<MeshRenderer>("MeshRenderer");
                renderCommandBuffer->DrawMesh(mr->mesh, mr->gameObject->transform.worldMatrix, *mr->material);
            }
        }

        global.renderThread->SubmitCommandBuffer(*renderCommandBuffer);
        sleep(1);
    }
}
LogicThread::LogicThread(QObject* parent):QThread(parent)
{
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
void LogicThread::GetCameras(std::vector<RenderItem<GameObject>>& vector)
{
    vector.clear();
    for (GameObject::ChildIterator i = configuration.sceneObject.GetStartChildIterator(), end = configuration.sceneObject.GetEndChildIterator(); i != end; i++)
    {
        GetCamerasDFS(vector, *i);
    }

}
void LogicThread::GetCamerasDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject)
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

void LogicThread::GetMeshRenderers(std::vector<RenderItem<GameObject>>& vector)
{
    vector.clear();
    for (GameObject::ChildIterator i = configuration.sceneObject.GetStartChildIterator(), end = configuration.sceneObject.GetEndChildIterator(); i != end; i++)
    {
        GetMeshRenderersDFS(vector, *i, glm::mat4(1));
    }

}
void LogicThread::GetMeshRenderersDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix)
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
void LogicThread::GetLights(std::vector<RenderItem<GameObject>>& vector)
{
    vector.clear();
    for (GameObject::ChildIterator i = configuration.sceneObject.GetStartChildIterator(), end = configuration.sceneObject.GetEndChildIterator(); i != end; i++)
    {
        GetLightsDFS(vector, *i, glm::mat4(1));
    }

}
void LogicThread::GetLightsDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix)
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
