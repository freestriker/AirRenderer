#pragma once
class RenderThread;
class LoadThread;
class LogicThread;
class Time;
class Global
{
public:
	RenderThread* renderThread;
	LoadThread* loadThread;
	LogicThread* logicThread;
	Time* time;
};
extern Global global;
