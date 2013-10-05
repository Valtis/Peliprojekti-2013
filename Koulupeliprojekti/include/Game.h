#pragma once

#include "Level/LevelManager.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	void Initialize();
	void RunMainLoop();
	void UpdateGameState();
	void Draw();

	LevelManager m_levelManager;
};

