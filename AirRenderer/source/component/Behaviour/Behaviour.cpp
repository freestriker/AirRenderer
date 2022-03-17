#include <include/component/Behaviour/Behaviour.h>

Behaviour::Behaviour(): Component("Behaviour")
{
}

Behaviour::~Behaviour()
{
	typeName = "DestoriedBehaviour";
}

void Behaviour::Awake()
{
}

void Behaviour::Start()
{
}

void Behaviour::Update()
{
}

void Behaviour::OnAddAsComponent()
{
	Start();
}
