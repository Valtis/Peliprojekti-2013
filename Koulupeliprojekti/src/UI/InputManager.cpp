#include <utility>
#include <vector>
#include "UI/InputManager.h"
#include "UI/Controller/Controller.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::HandleInput()
{
	for(auto const mController : mControllers)
	{
		Command* command = mController->HandleInput();
		if (command != nullptr)
			for (auto handler : mInputHandlers)
			{
				if( !handler.second(command) )
					break;
			}
	}
}

void InputManager::AddController(Controller* controller)
{
	if (controller!=nullptr)
		mControllers.push_back(controller);
}

void InputManager::RegisterInputHandler(InputHandler handler, int priority)
{
	mInputHandlers.push_back(std::make_pair(priority, handler));
	SortHandlers();
}

void InputManager::SortHandlers()
{
// TODO: Sort Handlers
}
