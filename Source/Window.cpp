#include "Window.h"
#include <GLEW\glew.h>
#include <iostream>
#include "Logger.h"
#include "SOIL\SOIL.h"

namespace Nova
{

	unsigned int  Window::m_height  = 0;
	unsigned int  Window::m_width   = 0;
	bool		  Window::m_vsync   = true;
	SDL_Window*   Window::m_window  = 0;
	SDL_GLContext Window::m_context = 0;

	Window::Window()
	{
		this->m_name = "Nova";
		this->m_width = 900;
		this->m_height = 600;
		this->m_settings = NovaWindowSettings::OPENGL;
	}

	Window::Window(const std::string& windowName, unsigned int width, unsigned int height)
	{
		this->m_name = windowName;
		this->m_width = width;
		this->m_height = height;
	}

	Window::Window(const std::string& windowName, unsigned int width, unsigned int height,NovaWindowSettings settings)
	{
		this->m_name = windowName;
		this->m_width = width;
		this->m_height = height; 
		this->m_settings = settings;
	}


	Window::~Window()
	{
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
		std::cout << "the window and the opengl context was destroyed" << std::endl;
	}

	bool Window::Init()
	{
		// initialise sdl video mode
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "sdl video could not initialise" << std::endl;
		}
	
		// set opengl version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


		// turn on double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		
		// set z buffer to 24 bit (system dependent might need to change to 16 or 32 bit)
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		m_window = SDL_CreateWindow(m_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		if (!m_window) std::cout << "SDL2 window creation failed" << std::endl;

		// create the opengl context and assign it to the sdl2 window
		m_context = SDL_GL_CreateContext(m_window);

		glewExperimental = true;
		GLenum err = glewInit();

		bool openglVersionOK = CheckSystemCompatability();

		if (!openglVersionOK)
		{
			return openglVersionOK;
		}

		if (err != GLEW_OK)
		{
			Logger::ErrorBlock("GLEW init failed", "Error: get glew error string", true);
			std::getchar();
			return false;
		}

		glViewport(0, 0, m_width, m_height);

		//glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		

		SetMousePosition(GetWidth() / 2, GetHeight() / 2);

		return openglVersionOK;
	}

	void Window::SwapBackBuffer()
	{
		SDL_GL_SetSwapInterval(m_vsync);
		SDL_GL_SwapWindow(m_window);
	}

	void Window::SetCursorVisablity(bool value)
	{
		SDL_ShowCursor(value);
	}

	unsigned int Window::GetWidth()
	{
		return m_width;
	}

	unsigned int Window::GetHeight()
	{
		return m_height;
	}

	float Window::GetAspectRatio()
	{
		return (static_cast<float>(m_width) / static_cast<float>(m_height));
	}

	void Window::SetMousePosition(int x, int y)
	{
		SDL_WarpMouseInWindow(m_window, x, y);
	}

	void Window::SetMousePosition(const glm::vec2& pos)
	{
		SDL_WarpMouseInWindow(m_window, static_cast<float>(pos.x), static_cast<float>(pos.y));
	}

	bool Window::CheckSystemCompatability()
	{
		const char* version			   = (char*)glGetString(GL_VERSION);
		const char* shadinglangVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		const char* renderer		   = (char*)glGetString(GL_RENDERER);
		const char* vendor			   = (char*)glGetString(GL_VENDOR);

		int major = 0;
		int minor = 0;

		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		major *= 100;
		minor *= 10;
		major += minor;
		
		if (major < 450)
		{
			std::string error = "Error:           The OpenGL version required is not supported\nCurrent Version: " + std::string(version) + 
								"\nRequired:        4.5.0\n"+
								"Solutions:       check if the Opengl drivers are up to date\n                 or get a new graphics card\n\nPress any key to exit";

			Logger::ErrorBlock("System Requirments Error", error, true);

			std::getchar();

			return false;
		}
		
			Logger::InfoBlock("System Info",
				"Opengl Version:           " + std::string(version) + "\n" +
				"Shading Language Version: " + std::string(shadinglangVersion) + "\n" +
				"Renderer:                 " + std::string(renderer) + "\n" +
				"Vendor:                   " + std::string(vendor)
				, true);
		
		return true;

	}

	void Window::SetVSync(bool value)
	{
		m_vsync = value;
	}

	void Window::TakeScreenShot()
	{
		int save_result = SOIL_save_screenshot
		(
			"awesomenessity.bmp",
			SOIL_SAVE_TYPE_BMP,
			0, 0, 1024, 768
		);
	}

}
