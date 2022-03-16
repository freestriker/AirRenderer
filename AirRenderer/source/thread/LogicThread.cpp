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
#include <include/utils/IntersectionTester.h>
#include <include/utils/Time.h>
void LogicThread::run()
{
    while (true)
    {
        global.time->Refresh();
        double launchTime = global.time->GetLaunchDuration();
        double deltaTime = global.time->GetDeltaDuration();
        std::string lt = "LaunchTime: " + std::to_string(launchTime);
        std::string dt = "DeltaTime: " + std::to_string(deltaTime);
        qDebug() << QString::fromStdString(lt) << QString::fromStdString(dt);
        RenderCommandBufferBuilder renderCommandBufferBuilder = RenderCommandBufferBuilder();
        if (componentListMap.count("Light"))
        {
            for (CrossLinkedNodeColItertor iter = componentListMap["Light"]->GetItertor(); iter.IsVaild(); ++iter)
            {
                Light* light = iter.Node<Light>();
                renderCommandBufferBuilder.AddLight(*light);

            }
        }
        if (componentListMap.count("Camera") && componentListMap.count("MeshRenderer"))
        {
            CrossLinkedColHead* cameraHead = componentListMap["Camera"];
            CrossLinkedColHead* meshRendererHead = componentListMap["MeshRenderer"];
            for (CrossLinkedNodeColItertor iter = cameraHead->GetItertor(); iter.IsVaild(); ++iter)
            {
                Camera* camera = iter.Node<Camera>();
                renderCommandBufferBuilder.SetCamera(*camera);
                glm::vec4 clipPlanes[6]{ glm::vec4(0, 0, 0, 0) };
                camera->ClipPlanes(clipPlanes);
                IntersectionTester intersectionTester = IntersectionTester(clipPlanes, 6);
                glm::mat4 viewMatrix = glm::inverse(camera->gameObject->transform.worldMatrix);
                for (CrossLinkedNodeColItertor iter = meshRendererHead->GetItertor(); iter.IsVaild(); ++iter)
                {
                    MeshRenderer* mr = iter.Node<MeshRenderer>();
                    glm::mat4 wvMatrix = viewMatrix * mr->gameObject->transform.worldMatrix;
                    mr->mesh->WaitForLoad();
                    if (intersectionTester.Check(mr->mesh->boundingBox->boundryVertexes, 8, wvMatrix))
                    {
                        mr->Render(renderCommandBufferBuilder);
                    }
                    else
                    {
                        std::string s = "Cull the gameobject called: " + mr->gameObject->name + ".";
                        qDebug() << QString::fromStdString(s);
                    }
                }
            }
        }

        global.renderThread->SubmitCommandBuffer(renderCommandBufferBuilder.BuildCommandBuffer());
        RealDestory();
        msleep(10);
    }
}
LogicThread::LogicThread(QObject* parent):QThread(parent)
{
    componentListMap = std::map<std::string, CrossLinkedColHead*>();
}

void LogicThread::Init()
{
    global.time->Launch();
    GameObject* camera = new GameObject("Camera");
    configuration.sceneObject.AddChild(camera);
    camera->AddComponent(new PerspectiveCamera());
    //camera->AddComponent(new OrthographicCamera());
    //camera->transform.SetTranslation(glm::vec3(0, 0, 5));
    camera->transform.SetTranslationRotationScale(glm::vec3(0, 0, 15), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(1, 1, 1));

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
    go0->transform.SetTranslationRotationScale(glm::vec3(0, 0, 0), glm::quat(glm::vec3(1.5707963267948966, 0, 0)), glm::vec3(5, 5, 5));
    go0->AddComponent(new MeshRenderer("../../Resources/Model/Sphere_Wall_Normal.ply"));
    Destory(go0);
    go0 = new GameObject("go0");
    configuration.sceneObject.AddChild(go0);
    go0->transform.SetTranslationRotationScale(glm::vec3(0, 0, 0), glm::quat(glm::vec3(1.5707963267948966, 0, 0)), glm::vec3(5, 5, 5));
    go0->AddComponent(new MeshRenderer("../../Resources/Model/Sphere_Wall_Normal.ply"));
    //go0->RemoveComponent<MeshRenderer>("MeshRenderer");
    //go0->AddComponent(new MeshRenderer("../../Resources/Model/Cube_Wall_Normal.ply"));
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
    go133->transform.SetTranslation(glm::vec3(0, 0, -3));
    go133->transform.SetScale(glm::vec3(19, 19, 1));
    //go133->transform.SetRotation(glm::vec3(0.2617993877991494, 0.2617993877991494, 0.2617993877991494));
    //go133->transform.SetScale(glm::vec3(2, 2, 1));
    go133->AddComponent(new MeshRenderer("../../Resources/Model/Flat_Wall_Normal.ply"));
    //go133->AddComponent(new MeshRenderer("../../Resources/Model/Triangle_Wall_Normal.ply"));

    GameObject* go134 = new GameObject("go134");
    go13->AddChild(go134);
    go134->transform.SetTranslation(glm::vec3(0, 0, 100));
    go134->AddComponent(new MeshRenderer("../../Resources/Model/Cube_Wall_Normal.ply"));

}
void LogicThread::RealDestory()
{
    for (int i = 0; i < destoryComponentVector.size(); i++)
    {
        delete destoryComponentVector[i];
    }
    destoryComponentVector.clear();
    for (int i = 0; i < destoryGameObjectVector.size(); i++)
    {
        delete destoryGameObjectVector[i];
    }
    destoryGameObjectVector.clear();
}

void LogicThread::Destory(Component* component)
{
    component->active = false;
    global.logicThread->destoryComponentVector.push_back(component);
}

void LogicThread::Destory(GameObject* gameObject)
{
    gameObject->active = false;
    gameObject->RemoveSelf();
    std::vector<Component*> cs = std::vector<Component*>();
    gameObject->RemoveAllComponent<Component>(cs);
    for each (Component * component in cs)
    {
        component->active = false;
        global.logicThread->componentListMap[component->typeName]->RemoveNode(component);
    }
    global.logicThread->destoryComponentVector.insert(global.logicThread->destoryComponentVector.begin(), cs.begin(), cs.end());
    global.logicThread->destoryGameObjectVector.push_back(gameObject);

}
