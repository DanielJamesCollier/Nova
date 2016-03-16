#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"
#include <iostream>

namespace Nova
{
	SceneManager::SceneManager()
	{
		
	}

	SceneManager::~SceneManager()
	{
		for (unsigned int i = 0; i < m_scenes.size(); i++)
		{
			delete m_scenes[i];
		}
		m_scenes.clear();
	}

	void SceneManager::AddScene(Scene* scene)
	{
		if (m_scenes.size() == 0)
		{
			scene->Initialise();
		}

		m_scenes.push_back(scene);
	}

	void SceneManager::InitialiseScene(int scene)
	{
		m_scenes[scene]->Initialise();
	}

	int SceneManager::GetSceneCount() const
	{
		return m_scenes.size();
	}

	void SceneManager::ProcessInput()
	{
		for (unsigned int i = 0; i < m_scenes.size(); i++)
		{
			if (InputManager::IsKeyReleased(SDLK_1 + i))
			{
				m_current = i;
				InitialiseScene(m_current);
			}
		}
	}

	void SceneManager::UpdateCurrent(float delta)
	{
		m_scenes[m_current]->Update(delta);
	}

	void SceneManager::UpdateAll(float delta)
	{
		for (unsigned int i = 0; i < m_scenes.size(); i++)
		{
			m_scenes[i]->Update(delta);
		}
	}

	void SceneManager::RenderCurrent()
	{
		m_scenes[m_current]->Render();
	}

	Scene* SceneManager::GetActiveScene() const
	{
		return m_scenes[m_current];
	}

}
