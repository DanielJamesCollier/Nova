#pragma once
#include <SDL2\SDL.h>
#include <string>
#include <GLM\glm.hpp>

namespace Nova
{
	enum NovaWindowSettings
	{
		OPENGL,
		OPENGLES,
		DIRECTX,
		VULKAN,
		BORDERLESS,
		RESIZABLE,
	};

	class Window
	{
	public:
		Window();
		Window(const std::string& windowName , unsigned int width, unsigned int height);
		Window(const std::string& windowName, unsigned int width, unsigned int height, NovaWindowSettings settings);
		~Window();

		void SwapBackBuffer();
		void static SetCursorVisablity(bool value);

		static unsigned int GetWidth();
		static unsigned int GetHeight();
		static float GetAspectRatio();
		static void         SetMousePosition(int x, int y);
		static void			SetVSync(bool value);
		
		bool Init();

		static void TakeScreenShot();	

	private:
		static unsigned int m_width, m_height;
		NovaWindowSettings m_settings;

		static SDL_Window   *m_window;
		static SDL_GLContext m_context;

		static bool m_vsync;

		std::string m_name;

		
		bool CheckSystemCompatability();
	};
}

