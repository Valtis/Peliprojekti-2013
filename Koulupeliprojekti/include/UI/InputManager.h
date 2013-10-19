#pragma once
#include <utility>
#include <vector>
#include <functional>
#include "UI/Controller/Controller.h"

class InputManager
{
public:
	InputManager();
	~InputManager();
	typedef std::function<bool(Command* command)> InputHandler;
	typedef std::pair<int, InputHandler> InputHandlerNode;
	void HandleInput();
	void AddController(Controller* controller);
	void RegisterInputHandler(InputHandler, int priority);
private:
	void SortHandlers();
	std::vector<InputHandlerNode> mInputHandlers;
	std::vector<Controller*> mControllers;
};
