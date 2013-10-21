#include <utility>
#include <vector>
#include "UI/InputManager.h"
#include "UI/Controller/KeyboardController.h"
#include "UI/Controller/MouseController.h"

InputManager::InputManager()
{
AddController(new MouseController());
AddController(new KeyboardController());
}

InputManager::~InputManager()
{
}

void InputManager::HandleInput(const SDL_Event& event)
{
	for(auto const mController : mControllers)
	{
		Command* command = mController->HandleInput(event);
		if (command != nullptr)
		{
			for (auto handler : mInputHandlers)
			{
				if( handler.second(command) )
				{
					break;
				}
			}
			delete command;
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
