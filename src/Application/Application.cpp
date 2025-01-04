#include "Application.h"
#include "../Resource/TextureManager.h"
#include "../Resource/ShaderManager.h"
#include "../Render/Renderer.h"
#include "../Player/Player.h"
#include "../Player/Controller.h"
#include <functional>
#include <string.h>

#include <glm/gtc/matrix_transform.hpp>

float Application::FPS = 100.0f;
FastNoiseLite Application::noise;

Application::Application(int argc, char* argv[])
{
	m_window = new Window(1600, 1000, "GLCraft");

	TextureManager::GetInstance().LoadTextures();
	ShaderManager::GetInstance().LoadShaders();
	Renderer::GetInstance().init();

	m_window->setMouseMoveEvent(Controller::MouseMove);
	m_window->setWindowResizeEvent(Controller::WindowResize);
	m_window->setMousePressEvent(Controller::MousePress);

	//----------------Debug--------------
	Player::PlayerInfo info;
	info.position = glm::vec3(0, 0, 0);
	info.front = glm::vec3(0, 0, -1);
	info.height = 0.0f;
	info.move_speed = 25.0f;
	Player::GetInstance().init(info);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetSeed(520);
	
	world = new World(Player::GetInstance().getPosition());
}

Application::~Application()
{
	delete m_window;
}

int Application::run()
{
	while (!m_window->shouldClose())
	{
		float time = glfwGetTime();

		m_window->update();
		Controller::KeyListen(m_window->getGlfwWindow());
		Renderer::GetInstance().drawBlocks();
		Renderer::GetInstance().drawCrosshair();
		Renderer::GetInstance().drawPlanet();
		world->updateRenderMeshes();
		world->updatePlanet();
		
		FPS = 1.0f / (glfwGetTime() - time);
		printInfo();
	}
	return 0;
}

void Application::printInfo()
{
	static int tick = 0;
	if(++ tick > 10000)
	{
		std::cout << "<GLCraft> " << "fps: " << GetFps() << "   square count: " << Renderer::GetInstance().getSquareCount() << std::endl;
		tick = 0;
	}
}

float Application::GetFps() { return FPS; }