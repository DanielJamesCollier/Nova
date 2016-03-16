#pragma once
#include <SDL2\SDL.h>
#include <string>
#include <GLM\glm.hpp>

namespace Nova
{
	// window size stored seperately to avoid register switching

	class Window
	{

		static unsigned int m_width, m_height;

		static SDL_Window   *m_window;
		static SDL_GLContext m_context;

		static bool m_vsync;

		std::string m_name;

	public:
		Window(const std::string& windowName , unsigned int width, unsigned int height);
		~Window();
		bool Init();

		void SwapBackBuffer();
		void static SetCursorVisablity(bool value);

		static unsigned int GetWidth();
		static unsigned int GetHeight();
		static float        GetAspectRatio();
		static void         SetMousePosition(int x, int y);
		static void			SetVSync(bool value);
		
		static void TakeScreenShot();	

	private:
		

		
		bool IsMinOpenglVersionSupported();
	};
}

