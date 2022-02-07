#include "RenderThread.h"
#include <vector>

RenderThread::RenderThread(QObject* parent) :QThread(parent)
{
    timer = new QTimer(this);
    renderer = new Renderer();
    GameObject* camera = configuration.sceneObject.AddChild();
    camera->camera = new Camera();
    //camera->transform.translation.z += 5;

    GameObject* go1 = configuration.sceneObject.AddChild();
    go1->transform.translation.x += 5;

    GameObject* go11 = go1->AddChild();
    go11->transform.translation.y += 5;
    go11->transform.translation.z -= 1;
    go11->meshRenderer = new MeshRenderer();

    GameObject* go12 = go1->AddChild();
    go12->transform.translation.y -= 5;
    go12->transform.translation.z -= 1;
    go12->meshRenderer = new MeshRenderer();

    GameObject* go13 = go1->AddChild();
    go13->transform.translation.x -= 5;
    go13->transform.translation.z -= 1;
    go13->meshRenderer = new MeshRenderer();
}

void RenderThread::Run()
{
    Render();
    timer->setInterval(900);
    connect(timer, SIGNAL(timeout()), this, SLOT(Render()));
    timer->start();
    this->exec();
}
void RenderThread::GetCameras(std::vector<RenderItem<GameObject>>& vector)
{
    vector.clear();
    for (GameObject::ChildIterator i = configuration.sceneObject.GetStartChildIterator(), end = configuration.sceneObject.GetEndChildIterator(); i != end; i++)
    {
        GetCamerasDFS(vector, *i, glm::mat4(1));
    }

}
void RenderThread::GetCamerasDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix)
{
    glm::mat4 matrix = parentMatrix * gameObject->transform.TranslationMatrix() * gameObject->transform.RotationMatrix();
    for (GameObject::ChildIterator i = gameObject->GetStartChildIterator(), end = gameObject->GetEndChildIterator(); i != end; i++)
    {
        GetCamerasDFS(vector, *i, matrix);
    }
    if (gameObject->camera)
    {
        vector.push_back(RenderItem<GameObject>(gameObject, matrix));
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
    if (gameObject->meshRenderer)
    {
        vector.push_back(RenderItem<GameObject>(gameObject, matrix));
    }
}

void RenderThread::Render()
{
    //renderer->Render();
    //renderer->Display();

    std::vector<RenderItem<GameObject>> cameraItems = std::vector<RenderItem<GameObject>>();
    GetCameras(cameraItems);
    std::vector<RenderItem<GameObject>> meshRendererItems = std::vector<RenderItem<GameObject>>();
    GetMeshRenderers(meshRendererItems);
    for (RenderItem<GameObject> cameraItem : cameraItems)
    {
        
        LogMatrix(cameraItem.transformationMatrix);
        glm::mat4 observeMatrix = glm::inverse(cameraItem.transformationMatrix);
        LogMatrix(observeMatrix);
        glm::mat4 projectionMatrix = cameraItem.item->camera->ProjectionMatrix();
        LogMatrix(projectionMatrix);
        for (RenderItem<GameObject> meshRendererItem : meshRendererItems)
        {
            glm::mat4 mvpMatrix = projectionMatrix * observeMatrix * meshRendererItem.transformationMatrix;
            LogMatrix(mvpMatrix);
            glm::mat4 screenMatrix = configuration.GetScreenMatrix();
            LogMatrix(screenMatrix);

            meshRendererItem.item->meshRenderer->Render(mvpMatrix, screenMatrix);
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

}

void RenderThread::LogMatrix(glm::mat4 matrix)
{
    qDebug() << "Matrix :";
    for (int i = 0; i < 4; i++)
    {
        QString s = "";
        for (int j = 0; j < 4; j++)
        {
            s += QString::number(matrix[j][i], 'f', 6);
            s += " ";
        }
        qDebug() << s;
    }
}