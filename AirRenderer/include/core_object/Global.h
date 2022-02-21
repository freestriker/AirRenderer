#pragma once
class RenderThread;
class LoadThread;
class Global
{
public:
	RenderThread* renderThread;
	LoadThread* loadThread;
};
extern Global global;
