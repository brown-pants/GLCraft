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

	inline Window* getWindow() const { return m_window; }
	
	static float GetFps();

	static FastNoiseLite& GetNoise() { return noise; }

	static Application* GetApp() { return app; }

private:
	static Application* app;

	Window* m_window;
	World *world;
	static float FPS;
	static FastNoiseLite noise;


	void printInfo();
};
#endif