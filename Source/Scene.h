#pragma once
#include "Camera.h"
#include "Lights.h"
#include "GLTexture.h"
#include "ResourceManager.h"
#include "Logger.h"
#include <GLM\glm.hpp>
#include <vector>

namespace Nova
{

	class GeometryPass;

	class Scene
	{
	public:
		Scene()
		{
			m_cams.push_back(Camera(glm::vec3(0, 0, 0), 70.0f, Window::GetAspectRatio(), 0.001f, 1000.0f));
			m_skytexture = ResourceManager::GetCubeMap("defaultSkyBox");
		}
		~Scene()
		{

		}

		virtual void Initialise() = 0;

		void AddCamera(const Camera& cam)
		{
			m_cams.push_back(cam);
		}

		Camera& GetActiveCamera()
		{
			return m_cams[m_activeCam];
		}

		void UpdateActiveCamera(float delta)
		{
			m_cams[m_activeCam].Update(delta);
		}

		void UpdateAllCameras(float delta)
		{
			for (Camera c : m_cams)
			{
				c.Update(delta);
			}
		}

		GLTexture* GetSkyTexture()
		{
			if (m_skytexture == nullptr)
			{
				// INFO: THIS CLASS SHOULD BE SUBCLASSED ONLY SO THIS SHOULD NEVER HAPPEN, FIND A FIX
			}

			return m_skytexture;
		}

		virtual void Update(float delta)		 = 0;
		virtual void Render(GeometryPass* gPass) = 0;

	protected:

		void SetInitialised(bool value)
		{
			m_initialised = value;
		}

		bool GetInitialised()
		{
			return m_initialised;
		}

		void SetSkyboxCubeMap(GLTexture* skybox)
		{
			if (skybox->type != GL_TEXTURE_CUBE_MAP)
			{
				Logger::GetInstance().ErrorBlock("Scene Error", "the skybox could not be set at the texture was not a cubemap", true);
				return;
			}
			m_skytexture = skybox;
		}

	private:
		std::vector<Camera> m_cams;
		unsigned int m_activeCam = 0;
		GLTexture* m_skytexture;

		bool m_initialised = false;

		void SetActiveCamera(unsigned int i)
		{
			m_activeCam = i;
		}

	};

}

