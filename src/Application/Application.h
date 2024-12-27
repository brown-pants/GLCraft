#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#include "Window.h"
#include "../World/World.h"

class Application
{
public:
	Application(int argc, char* argv[]);
	~Application();
	int run();
	void printInfo();
	
	static float GetFps();

private:
	Window* m_window;
	World *world;
	static float FPS;
};
#endif