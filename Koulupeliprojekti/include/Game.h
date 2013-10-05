#pragma once
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
};

