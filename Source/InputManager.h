#pragma once
#include <unordered_map>
#include <vector>
#include <GLM/glm.hpp>
#include "Window.h"

namespace Nova
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		static void Init();

		static void PressKey(unsigned int key);
		static void ReleaseKey(unsigned int key);

		static unsigned int GetLastReleasedKey();
		static bool IsKeyPressed(unsigned int key);
		static bool IsKeyReleased(unsigned int key);
		static glm::vec2& GetMousePosition();
		static float GetMouseXPosition();
		static float GetMouseYPosition();
		static float GetMouseDeltaX();
		static float GetMouseDeltaY();
		static const glm::vec2& GetMouseDeltaXY();
		static bool WasMouseMoved();
		static bool IsFocused();

		static void WindowWasResized();

		static void Update(bool& running);

		

		
	private:
		static std::unordered_map<unsigned int, bool> m_keys;
		static std::vector<unsigned int> m_releasedKeys; // released the last frame
		static glm::vec2 m_mousePosition;
		static glm::vec2 m_mouseDelta;
		static glm::vec2 m_windowCenterXY;
		static bool      m_mouseLockedInWindow;
		static bool      m_mouseMoved;

	
	};
}
