#pragma once
#include <vector>

namespace Nova
{
	class Scene;

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		void AddScene(Scene* scene);

		void InitialiseScene(int scene);

		void ProcessInput();
		void UpdateCurrent(float delta);
		void UpdateAll(float delta);
		void RenderCurrent();

		

		Scene* GetActiveScene() const;

		int GetSceneCount() const;
		
	private:
		std::vector<Scene*> m_scenes;
		int				    m_current = 0;
	};

}
