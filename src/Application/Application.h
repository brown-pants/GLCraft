#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#include "Window.h"
#include "../World/World.h"
#include <FastNoiseLite.h>
#include <thread>
#include <SQLiteCpp/SQLiteCpp.h>
#include <unordered_map>

class Application
{
public:
	Application(int argc, char* argv[]);
	~Application();
	int run();
	void changeBlock(int chunk_x, int chunk_z, int block_x, int block_y, int block_z, Block_Type block_type);
	int blockChanged(int chunk_x, int chunk_z, int block_x, int block_y, int block_z);

	inline Window* getWindow() const { return m_window; }
	
	static float GetFps();

	static Application* GetApp() { return app; }

private:
	static Application* app;
	static float FPS;
	bool isLoading;
	std::thread *m_loadThread;
	Window* m_window;
	World *world;
	SQLite::Database *curDB;

	struct BlockPosition
	{
		int chunk_x;
		int chunk_z;
		int block_x;
		int block_y;
		int block_z;
		bool operator==(const BlockPosition& other) const;
		struct Hash
		{
			std::size_t operator()(const BlockPosition& pos) const;
		};

	};
	std::unordered_map<BlockPosition, Block_Type, BlockPosition::Hash> m_changedBlocks;

	std::string menu();
	std::string newWorld();
	std::string chooseWorld(const std::vector<std::string> &worlds);
	void clearScreen();
	std::vector<std::string> getNonHiddenFiles(const std::string& directory);
	bool createWorld(const std::string &name, int seed);
	bool loadWorld(const std::string &name);
	void saveGameInfo();
	void InitImGui();
	void renderImGui();
};
#endif