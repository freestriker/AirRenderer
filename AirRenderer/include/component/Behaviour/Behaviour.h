#pragma once
#include <include/component/Component.h>
class Behaviour:public Component
{
public:
	Behaviour();
	virtual ~Behaviour();
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	void OnAddAsComponent()override;
 };