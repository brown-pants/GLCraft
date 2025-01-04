#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#include "Window.h"
#include "../World/World.h"
#include <FastNoiseLite.h>

class Application
{
public:
	Application(int argc, char* argv[]);
	~Application();
	int run();
	void printInfo();
	
	static float GetFps();

	static FastNoiseLite& GetNoise() { return noise; }

private:
	Window* m_window;
	World *world;
	static float FPS;
	static FastNoiseLite noise;
};
#endif