#include <algorithm>
#include <utility>
#include <vector>
#include "UI/InputManager.h"
#include "UI/Controller/KeyboardController.h"
#include "UI/Controller/MouseController.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::Initialize()
{
	AddController(new MouseController());
	AddController(new KeyboardController());
	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		// add joystick No. i
	}
}

void InputManager::HandleInput(const SDL_Event& event) const
{
	for(auto const mController : mControllers)
	{
		Command* command = mController->HandleInput(event);
		if (command != nullptr)
		{
			for (const auto handler : mInputHandlers)
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
	std::stable_sort(mInputHandlers.begin(),mInputHandlers.end(),[&]( const std::pair<int, InputHandler> & a, const std::pair<int, InputHandler>& b) { return a.first > b.first; });
// TODO: Sort Handlers
}
