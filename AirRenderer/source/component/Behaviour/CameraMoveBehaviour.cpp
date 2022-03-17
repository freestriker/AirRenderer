#include <include/component/Behaviour/CameraMoveBehaviour.h>
#include <include/core_object/GameObject.h>
#include <include/utils/Time.h>

CameraMoveBehaviour::CameraMoveBehaviour()
{
	transform = nullptr;
	speed = 0.0;
	direction = glm::vec3(0, 0, 0);
}
CameraMoveBehaviour::~CameraMoveBehaviour()
{
	transform = nullptr;
}
void CameraMoveBehaviour::Start()
{
	transform = &(gameObject->transform);
}
void CameraMoveBehaviour::Update()
{
	glm::vec3 pre = transform->translation;
	pre = pre + direction * speed * float(global.time->GetDeltaDuration());
	transform->SetTranslation(pre);
}
