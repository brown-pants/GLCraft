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

Application::Application(int argc, char* argv[])
{
	m_window = new Window(1600, 1000, "GLCraft");
	
	TextureManager::GetInstance().LoadTextures();
	ShaderManager::GetInstance().LoadShaders();
	Renderer::GetInstance().init();

	m_window->setMouseMoveEvent(Controller::MouseMove);
	m_window->setWindowResizeEvent(Controller::WindowResize);

	//----------------Debug--------------
	Player::PlayerInfo info;
	info.position = glm::vec3(0, 0, 0);
	info.front = glm::vec3(0, 0, -1);
	info.height = 0.0f;
	info.move_speed = 5.0f;
	Player::GetInstance().init(info);

	// std::vector<glm::mat4> matrices;
	// matrices.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -1)));
	// std::vector<float> vOffsets;
	// vOffsets.push_back(1.0f);
	// Renderer::GetInstance().updateSquares(vOffsets, matrices);

	
	world = new World;
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
		world->update();
		
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
		std::cout << "<GLCraft> " << "fps: " << GetFps() << std::endl;
		tick = 0;
	}
}

float Application::GetFps() { return FPS; }