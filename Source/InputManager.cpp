#include "InputManager.h"

#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>

namespace Nova
{

	std::unordered_map<unsigned int, bool> InputManager::m_keys;
	std::vector<unsigned int> InputManager::m_releasedKeys;
	glm::vec2 InputManager::m_mousePosition;
	glm::vec2 InputManager::m_mouseDelta;
	glm::vec2 InputManager::m_windowCenterXY;
	bool InputManager::m_mouseLockedInWindow = false;
	bool InputManager::m_mouseMoved = false;

	InputManager::InputManager()
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::Init()
	{
		WindowWasResized();
	}

	unsigned int InputManager::GetLastReleasedKey()
	{
		if(m_keys.size() > 0)
		return m_releasedKeys.back();
		return -1;
	}

	void InputManager::WindowWasResized()
	{
		m_windowCenterXY = glm::vec2(Window::GetWidth() / 2, Window::GetHeight() / 2);
	}

	void InputManager::PressKey(unsigned int key)
	{
		m_keys[key] = true;
	}

	void InputManager::ReleaseKey(unsigned int key)
	{
		m_keys[key] = false;
		m_releasedKeys.push_back(key);
	}

	bool InputManager::IsKeyPressed(unsigned int key)
	{
		auto search = m_keys.find(key);

		if (search != m_keys.end()) return m_keys[key];

		return false;
	}

	bool InputManager::IsKeyReleased(unsigned int key)
	{
		if (std::find(m_releasedKeys.begin(), m_releasedKeys.end(), key) != m_releasedKeys.end())
		{
			return true;
		}

		return false;
	}
	
	glm::vec2& InputManager::GetMousePosition()
	{
		return m_mousePosition;
	}

	float InputManager::GetMouseXPosition()
	{
		return m_mousePosition.x;
	}

	float InputManager::GetMouseYPosition()
	{
		return m_mousePosition.y;
	}

	float InputManager::GetMouseDeltaX()
	{
		return m_mouseDelta.x;
	}

	float InputManager::GetMouseDeltaY()
	{
		return m_mouseDelta.y;
	}

	const glm::vec2& InputManager::GetMouseDeltaXY()
	{
		return m_mouseDelta;
	}

	bool InputManager::WasMouseMoved()
	{
		if (!m_mouseLockedInWindow) return false;
		return m_mouseMoved;
	}

	bool InputManager::IsFocused()
	{
		return m_mouseLockedInWindow;
	}

	void InputManager::Update(bool& running)
	{
		SDL_Event event;

		if (IsKeyPressed(SDLK_f))
		{
			m_mouseLockedInWindow = true;
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		else if (IsKeyPressed(SDLK_r))
		{
			m_mouseLockedInWindow = false;
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}

		m_mouseMoved = false;

		m_mouseDelta = glm::vec2();

		m_releasedKeys.clear();

		while (SDL_PollEvent(&event))
		{
		

			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;		

			case SDL_KEYDOWN:
				InputManager::PressKey(event.key.keysym.sym);
				break;

			case SDL_KEYUP:
				InputManager::ReleaseKey(event.key.keysym.sym);
				break;

			case SDL_MOUSEBUTTONDOWN:
				InputManager::PressKey(event.button.button);
				
				break;

			case SDL_MOUSEBUTTONUP:
				InputManager::ReleaseKey(event.button.button);
		
				break;

			case SDL_MOUSEMOTION:
				if (m_mouseLockedInWindow)
				{
					m_mousePosition = glm::vec2(event.button.x, event.button.y);
					m_mouseDelta = glm::vec2(event.motion.xrel, event.motion.yrel);
					m_mouseMoved = true;
				}
				break;
			}
		}
	}
}
