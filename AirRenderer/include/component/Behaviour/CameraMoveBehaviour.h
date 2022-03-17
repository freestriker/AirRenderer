#pragma once
#include <include/component/Behaviour/Behaviour.h>
#include <include/component/transform/Transform.h>
class CameraMoveBehaviour: public Behaviour
{
	Transform* transform;
public:
	float speed;
	glm::vec3 direction;
	CameraMoveBehaviour();
	~CameraMoveBehaviour();
	void Start()override;
	void Update()override;
};