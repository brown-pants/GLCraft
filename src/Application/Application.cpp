#include "Application.h"
#include "../Resource/TextureManager.h"
#include "../Resource/ShaderManager.h"
#include "../Render/Renderer.h"
#include "../Player/Player.h"
#include "../Player/Controller.h"
#include "../Math/Noise/PerlinNoise.h"
#include <functional>
#include <filesystem>
#include <string.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

Application* Application::app = nullptr;

float Application::FPS = 100.0f;

Application::Application(int argc, char* argv[])
{
	app = this;

	curDB = nullptr;
	std::string worldName = menu();

	m_window = new Window(1600, 1000, "GLCraft");
	world = new World();
	m_window->setWindowResizeEvent(Controller::WindowResize);

	TextureManager::GetInstance().LoadTextures();
	ShaderManager::GetInstance().LoadShaders();
	Renderer::GetInstance().init();

	isLoading = true;
	m_loadThread = new std::thread([this, worldName](){
		
		if (!loadWorld(worldName))
		{
			m_window->hide();
			m_window->close();
			return;
		}
		isLoading = false;
		m_window->setMouseMoveEvent(Controller::MouseMove);
		m_window->setMousePressEvent(Controller::MousePress);
		m_window->setScrollEvent(Controller::scrollWheel);
	});

	InitImGui();
}

Application::~Application()
{
	m_loadThread->join();
	delete m_loadThread;
	if (curDB)
	{
		saveGameInfo();
		delete curDB;
	}
	delete world;
	Renderer::GetInstance().clearMeshes();
	delete m_window;
}

int Application::run()
{
	while (!m_window->shouldClose())
	{
		if (isLoading)
		{
			m_window->update();
			Renderer::GetInstance().drawLoadingBackground();
			continue;
		}

		renderImGui();
		glm::vec3 skyColor = world->getSkyColor();
		glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
		m_window->update();
		Controller::KeyListen(m_window->getGlfwWindow());
		glEnable(GL_CULL_FACE);
		Renderer::GetInstance().drawBlocks();
		Renderer::GetInstance().drawPlanet();
		Renderer::GetInstance().drawWater();
		Renderer::GetInstance().drawCrosshair();
		world->updateRenderMeshes();
		world->updatePlanet();
		world->updateFlowWater();
		Player::GetInstance().physical();
	}
	world->stop();
	return 0;
}

void Application::changeBlock(int chunk_x, int chunk_z, int block_x, int block_y, int block_z, Block_Type block_type)
{
	m_changedBlocks[BlockPosition{chunk_x, chunk_z, block_x, block_y, block_z}] = block_type;
	try {
		SQLite::Statement upsert(*curDB,
		"INSERT OR REPLACE INTO change (chunk_x, chunk_z, block_x, block_y, block_z, block_type) "
		"VALUES (?, ?, ?, ?, ?, ?)");
		upsert.bind(1, chunk_x);
		upsert.bind(2, chunk_z);
		upsert.bind(3, block_x);
		upsert.bind(4, block_y);
		upsert.bind(5, block_z);
		upsert.bind(6, block_type);
		upsert.exec();
	} catch (const std::exception& e) {
        std::cerr << "SQLite exception: " << e.what() << std::endl;
    }
}

int Application::blockChanged(int chunk_x, int chunk_z, int block_x, int block_y, int block_z)
{
	auto iter = m_changedBlocks.find(BlockPosition{chunk_x, chunk_z, block_x, block_y, block_z});
	if (iter == m_changedBlocks.end())
	{
		return -1;
	}
	return iter->second;
}

float Application::GetFps() { return ImGui::GetIO().Framerate; }

std::string Application::menu()
{
	clearScreen();
	std::filesystem::create_directory("worlds");
	std::cout << "Hello GLCraft!" << std::endl;
	std::cout << "  1. new world" << std::endl;
	std::cout << "  2. old world" << std::endl;
	std::string cmd;
	while (true)
	{
		std::cout << "Input(1/2): ";
		std::cin >> cmd;
		if (cmd == "1")
		{
			clearScreen();
			std::cout << "Create a new world" << std::endl;
			return newWorld();
		}
		else if (cmd == "2")
		{
			auto worlds = getNonHiddenFiles("worlds");
			if (worlds.empty())
			{
				std::cout << "Please create a world." << std::endl;
				continue;
			}
			return chooseWorld(worlds);
		}
		else
		{
			std::cout << "invaild input" << std::endl;
		}
	}
	
}

std::string Application::newWorld()
{
	while (true)
	{
		std::string name;
		int seed;
		std::cout << "Input(world name): ";
		std::cin >> name;
		auto worlds = getNonHiddenFiles("worlds");
		if (std::find(worlds.begin(), worlds.end(), name) != worlds.end())
		{
			std::cout << "repeat." << std::endl;
			continue;
		}
		std::cout << "Input(seed): ";
		std::cin >> seed;
		if (createWorld(name, seed))
		{
			std::cout << "successful" << std::endl;
			return name;
		}
		else
		{
			continue;
		}
		break;
	}
	return "";
}

std::string Application::chooseWorld(const std::vector<std::string> &worlds)
{
	clearScreen();
	std::cout << "worlds:" << std::endl;
	for (int i = 0; i < worlds.size(); i ++)
	{
		std::cout << "  " << i + 1 << ". " << worlds[i] << std::endl;
	}
	std::string id;
	int worldId;
	while (true)
	{
		std::cout << "Input(world id): ";
		std::cin >> id;
		try {
        	int worldId = std::stoi(id);
			if (worldId <= 0 || worldId > worlds.size())
			{
				std::cout << "invaild input" << std::endl;
				continue;
			}
			return worlds[worldId - 1];
		} catch (const std::invalid_argument&) {
			std::cout << "invaild input" << std::endl;
			continue;
		} catch (const std::out_of_range&) {
			std::cout << "invaild input" << std::endl;
			continue;
		}
	}
}

void Application::clearScreen()
{
#if defined(__WIN32)
	system("cls");
#else
	system("clear");
#endif
}

std::vector<std::string> Application::getNonHiddenFiles(const std::string& directory) 
{
	std::vector<std::string> result;

	try {
		for (const auto& entry : std::filesystem::directory_iterator(directory)) {
			if (entry.is_regular_file()) {
				std::string filename = entry.path().filename().string();
				
				// 检查是否为隐藏文件
				if (!filename.empty() && filename[0] != '.') {
					result.push_back(filename);
				}
			}
		}
	} catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Error accessing directory: " << e.what() << std::endl;
	}

	std::sort(result.begin(), result.end());
	return result;
}

bool Application::createWorld(const std::string &name, int seed)
{
 	try {
        SQLite::Database db("worlds/" + name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        db.exec("CREATE TABLE IF NOT EXISTS info ("
                "seed INTEGER, "
				"sunAngle REAL, "
                "position_x REAL, "
                "position_y REAL, "
                "position_z REAL, "
                "front_x REAL, "
                "front_y REAL, "
                "front_z REAL)");

		db.exec("CREATE TABLE IF NOT EXISTS change ("
				"chunk_x INTEGER, "
				"chunk_z INTEGER, "
				"block_x INTEGER, "
				"block_y INTEGER, "
				"block_z INTEGER, "
				"block_type INTEGER, "
				"PRIMARY KEY (chunk_x, chunk_z, block_x, block_y, block_z))");

		SQLite::Statement insert(db,
            "INSERT INTO info (seed, sunAngle, position_x, position_y, position_z, "
            "front_x, front_y, front_z) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        insert.bind(1, seed);
        insert.bind(2, 0.0);
        insert.bind(3, 0.0);
        insert.bind(4, 256.0);
        insert.bind(5, 0.0);
        insert.bind(6, 0.0);
        insert.bind(7, 0.0);
        insert.bind(8, -1.0);
        insert.exec();
    } catch (const std::exception& e) {
        std::cerr << "SQLite exception: " << e.what() << std::endl;
        return false;
    }
	return true;
}

bool Application::loadWorld(const std::string &name)
{
 	try {
        curDB = new SQLite::Database("worlds/" + name, SQLite::OPEN_READWRITE);
        SQLite::Statement query(*curDB,
            "SELECT seed, sunAngle, position_x, position_y, position_z, "
            "front_x, front_y, front_z FROM info");

        if (query.executeStep()) 
		{
            int seed = query.getColumn(0);
            double sunAngle = query.getColumn(1);
            double pos_x = query.getColumn(2);
            double pos_y = query.getColumn(3);
            double pos_z = query.getColumn(4);
            double front_x = query.getColumn(5);
            double front_y = query.getColumn(6);
            double front_z = query.getColumn(7);
			Player::PlayerInfo info;
			info.position = glm::vec3(pos_x, pos_y, pos_z);
			info.front = glm::vec3(front_x, front_y, front_z);
			Player::GetInstance().init(info);
			PerlinNoise::Init(seed);
			// load changed blocks
			SQLite::Statement query(*curDB, "SELECT chunk_x, chunk_z, block_x, block_y, block_z, block_type FROM change");
			while (query.executeStep())
			{
				BlockPosition blockPos;
				blockPos.chunk_x = query.getColumn(0);
				blockPos.chunk_z = query.getColumn(1);
				blockPos.block_x = query.getColumn(2);
				blockPos.block_y = query.getColumn(3);
				blockPos.block_z = query.getColumn(4);
				int block_type = query.getColumn(5);
				m_changedBlocks.insert(std::pair<BlockPosition, Block_Type>(blockPos, (Block_Type)block_type));
			}
			world->init(Player::GetInstance().getInfo().position, sunAngle);
		}
		else
		{
			curDB = nullptr;
			return false;
		}
	} catch (const std::exception& e) {
        std::cerr << "SQLite exception: " << e.what() << std::endl;
        return false;
    }
	return true;
}

bool Application::BlockPosition::operator==(const BlockPosition& other) const
{
	return chunk_x == other.chunk_x && chunk_z == other.chunk_z && 
			block_x == other.block_x && block_y == other.block_y && block_z == other.block_z;
}

std::size_t Application::BlockPosition::Hash::operator()(const BlockPosition& pos) const
{
	std::size_t h1 = std::hash<int>{}(pos.chunk_x);
	std::size_t h2 = std::hash<int>{}(pos.chunk_z);
	std::size_t h3 = std::hash<int>{}(pos.block_x);
	std::size_t h4 = std::hash<int>{}(pos.block_y);
	std::size_t h5 = std::hash<int>{}(pos.block_z);
	return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
}

void Application::saveGameInfo()
{
	try {
		SQLite::Statement update(*curDB,
            "UPDATE info SET sunAngle = ?, position_x = ?, position_y = ?, position_z = ?, "
    		"front_x = ?, front_y = ?, front_z = ?");
        update.bind(1, world->sunAngle());
        update.bind(2, Player::GetInstance().getInfo().position.x);
        update.bind(3, Player::GetInstance().getInfo().position.y);
        update.bind(4, Player::GetInstance().getInfo().position.z);
        update.bind(5, Player::GetInstance().getInfo().front.x);
        update.bind(6, Player::GetInstance().getInfo().front.y);
        update.bind(7, Player::GetInstance().getInfo().front.z);
        update.exec();
	} catch (const std::exception& e) {
        std::cerr << "SQLite exception: " << e.what() << std::endl;
    }
}

void Application::InitImGui()
{
	// 1. 创建上下文
    ImGui::CreateContext();
    
    // 2. 设置样式（可选）
    ImGui::StyleColorsDark();
    
    // 3. 初始化平台后端
    if (!ImGui_ImplGlfw_InitForOpenGL(m_window->getGlfwWindow(), true)) {
        std::cerr << "Failed to initialize ImGui GLFW backend" << std::endl;
        return;
    }
    
    // 4. 初始化渲染后端
    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        std::cerr << "Failed to initialize ImGui OpenGL3 backend" << std::endl;
        return;
    }
	
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    io.FontGlobalScale = 1.5f; // 全局字体缩放
}

void Application::renderImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
	const glm::vec3 &playerPos = Player::GetInstance().getInfo().position;
	
    // 构建UI
    ImGui::Begin("GLCraft Menu");
	ImGui::Text("Press the \"ctrl\" to release the mouse.");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("square count: %d", Renderer::GetInstance().getSquareCount());
    ImGui::Text("current block: %s", Controller::getCurBlock().c_str());
    ImGui::Text("player position: (%.1f, %.1f, %.1f)", playerPos.x, playerPos.y, playerPos.z);

	// 可是太阳啊
    ImGui::SliderFloat(" Sun Angle", &World::RunningWorld->sunAngle(), 0.0f, 360.0f);
	
	// 移速
    ImGui::SliderFloat(" Move Speed", &Player::GetInstance().getInfo().move_speed, 0.001f, 1.0f);
    
	// 重力
    ImGui::SliderFloat(" Gravity", &Player::GetInstance().getInfo().gravity, 0.001f, 0.01f);
    
	// 跳跃
    ImGui::SliderFloat(" jump", &Player::GetInstance().getInfo().jump, -0.1f, -1.0f);

	// 物理
	ImGui::Checkbox(" Physical", &Player::GetInstance().getInfo().physical);

	// 多连跳
	ImGui::Checkbox(" Multiple Jump", &Player::GetInstance().getInfo().multipleJump);
    ImGui::End();
    
    // 渲染
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}