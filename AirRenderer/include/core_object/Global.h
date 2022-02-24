#pragma once
class RenderThread;
class LoadThread;
class LogicThread;
class Global
{
public:
	RenderThread* renderThread;
	LoadThread* loadThread;
	LogicThread* logicThread;
};
extern Global global;
