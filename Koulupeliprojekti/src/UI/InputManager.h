#pragma once
#include <utility>
#include <vector>
#include <functional>
#include <SDL.h>
#include "UI/Controller/Controller.h"

class InputManager
{
public:
	InputManager();
	~InputManager();
	typedef std::function<MessageHandling(Command* command)> InputHandler;
	typedef std::pair<int, InputHandler> InputHandlerNode;
	void HandleInput(const SDL_Event& event) const;
	void RegisterInputHandler(InputHandler, int priority);
	void Initialize();
private:
	void AddController(Controller* controller);
	void SortHandlers();
	std::vector<InputHandlerNode> mInputHandlers;
	std::vector<Controller*> mControllers;
};
