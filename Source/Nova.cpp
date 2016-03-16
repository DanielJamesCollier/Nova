#include "Nova.h"
#include "Window.h"
#include "ResourceManager.h"
#include "ShaderBinder.h"
#include "GLTexture.h"
#include "Utils.h"
#include "Material.h"
#include "Sphere.h"
#include "Logger.h"
#include "MathUtil.h"
#include "ECSTestScene.h"
#include "ProfileManager.h"
#include "Shader.h"
#include <SDL2\SDL.h>
#include <math.h>
#include <vector>
#include "TextureBinder.h"


int main(int argc, char* argv[])
{
	{
		Nova::Nova application(Nova::Window("Nova Engine v0.1", 1920, 1080));
		application.Dispose();
	}

	std::string pause;
	std::cin >> pause;

	return 0;
}

namespace Nova
{
	Nova::Nova(Window window)
		:
		m_window(window)	
	{
		if (m_window.Init())
		{
			Start();
		}
		else
		{
			Logger::GetInstance().ErrorBlock("Nova Engine", "Error: engine could not be started, system not supported", true);
			std::string pause;
			std::cout << "press any key to quit" << std::endl;
			std::cin >> pause;
		}
	}

	Nova::~Nova()
	{
		Dispose();
	}

	void Nova::Dispose()
	{
		if (m_disposed) return;
		m_disposed = true;
		Logger::GetInstance().InfoBlock("Nove Engine Shutdow", "shutdown", true);
		ResourceManager::Dispose();
	}

	void Nova::Start()
	{
		if (m_running) return;
		    m_running = true;

		ResourceManager::Init();
		InputManager::Init();
	
		m_sceneManager.AddScene(new ScratchPad::ECSTestScene());

		Run();
	}

	void Nova::Stop()
	{
		if (!m_running) return;
		m_running = false;

		ResourceManager::Dispose();
	}

	void Nova::Run()
	{
		int frameCount     = 0;
		int currentTime    = SDL_GetTicks();
		int startTime      = currentTime;
		int passedTime     = currentTime;
		int startTimeDelta = currentTime;

		// calculate fps
		while (m_running)
		{
			
			currentTime = SDL_GetTicks();
			passedTime  = currentTime - startTime;

			frameCount++;

			ProcessInput();
							
			Update((currentTime - startTimeDelta) * 0.001f);
			startTimeDelta = currentTime;
		
			Render();
			

			if (passedTime >= 1000)
			{
				Profiler::ProfileManager::m_FPS = frameCount;
				startTime = currentTime;
				frameCount = 0;

				std::cout << "TBPS: "  << TextureBinder::GetInstance().GetAndResetBindCount() << std::endl;
				std::cout << "SBPS: " << ShaderBinder::GetAndResetBindCount() << std::endl;
				Profiler::ProfileManager::Display();
			}
		}
		Stop();
	}

	void Nova::ProcessInput()
	{
		m_inputManager.Update(m_running);
		m_sceneManager.ProcessInput();

		if (InputManager::IsKeyPressed(SDLK_p))
		{
			m_displayVolumes = true;
		}
		else if (InputManager::IsKeyPressed(SDLK_o))
		{
			m_displayVolumes = false;
		}

		if (InputManager::IsKeyPressed(SDLK_v))
		{
			m_srgb = true;
		}
		else if (InputManager::IsKeyPressed(SDLK_c))
		{
			m_srgb = false;
		}

		if (InputManager::IsKeyReleased(SDLK_t))
		{
			Window::TakeScreenShot();
		}
	}

	void Nova::Update(float delta)
	{
		m_sceneManager.UpdateCurrent(delta);
	}

	void Nova::Render()
	{
		m_sceneManager.RenderCurrent();
		m_window.SwapBackBuffer();
	}

}



