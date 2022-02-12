#include "RenderThread.h"
#include <vector>
#include "Utils.h"

RenderThread::RenderThread(QObject* parent) :QThread(parent)
{
    timer = new QTimer(this);
    renderer = new Renderer();
    GameObject* camera = new GameObject("Camera");
    configuration.sceneObject.AddChild(camera);
    camera->AddComponent(new Camera());
    camera->transform.SetTranslation(glm::vec3(0, 0, 5));

    GameObject* go0 = new GameObject("go0");
    configuration.sceneObject.AddChild(go0);
    go0->transform.SetTranslationRotationScale(glm::vec3(-5, 0, 0), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(5, 5, 5));
    go0->AddComponent(new MeshRenderer("../../Model/Sphere_Wall_Normal.ply"));
    
    GameObject* go1 = new GameObject("go1");
    configuration.sceneObject.AddChild(go1);
    go1->transform.SetTranslation(glm::vec3(5, 0, 0));

    GameObject* go11 = new GameObject("go11");
    go1->AddChild(go11);
    go11->transform.SetTranslation(glm::vec3(0, 5, -1));
    go11->AddComponent(new MeshRenderer());

    GameObject* go12 = new GameObject("go12");
    go1->AddChild(go12);
    go12->transform.SetTranslation(glm::vec3(0, -5, -1));
    go12->AddComponent(new MeshRenderer());

    GameObject* go13 = new GameObject("go13");
    go1->AddChild(go13);
    go13->transform.SetTranslation(glm::vec3(-5, 0, 0));

    GameObject* go131 = new GameObject("go131");
    go13->AddChild(go131);
    go131->transform.SetTranslation(glm::vec3(0, 0, -1));
    go131->AddComponent(new MeshRenderer());

    GameObject* go132 = new GameObject("go132");
    go13->AddChild(go132);
    go132->transform.SetTranslation(glm::vec3(0.45, 0, -2));
    go132->AddComponent(new MeshRenderer());

    GameObject* go133 = new GameObject("go133");
    go13->AddChild(go133);
    go133->transform.SetTranslation(glm::vec3(0.9, 0, -3));
    go133->transform.SetScale(glm::vec3(15, 15, 1));
    go133->AddComponent(new MeshRenderer());
}

void RenderThread::Run()
{
    Render();
    //timer->setInterval(900);
    //connect(timer, SIGNAL(timeout()), this, SLOT(Render()));
    //timer->start();
    //this->exec();
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

void RenderThread::Render()
{
    std::vector<RenderItem<GameObject>> cameraItems = std::vector<RenderItem<GameObject>>();
    GetCameras(cameraItems);
    std::vector<RenderItem<GameObject>> meshRendererItems = std::vector<RenderItem<GameObject>>();
    GetMeshRenderers(meshRendererItems);
    for (RenderItem<GameObject> cameraItem : cameraItems)
    {
        
        //LogMatrix(cameraItem.transformationMatrix);
        glm::mat4 viewMatrix = glm::inverse(cameraItem.transformationMatrix);
        //LogMatrix(observeMatrix);
        glm::mat4 projectionMatrix = cameraItem.item->FindComponent<Camera>("Camera")->ProjectionMatrix();
        //LogMatrix(projectionMatrix);
        for (RenderItem<GameObject> meshRendererItem : meshRendererItems)
        {
            glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * meshRendererItem.transformationMatrix;
            //LogMatrix(mvpMatrix);
            glm::mat4 screenMatrix = configuration.GetScreenMatrix();
            //LogMatrix(screenMatrix);

            meshRendererItem.item->FindComponent<MeshRenderer>("MeshRenderer")->Render(mvpMatrix, screenMatrix);
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