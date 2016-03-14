#pragma once
#include "Camera.h"
#include "Lights.h"
#include "GLTexture.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "CTransform.h"
#include "StaticPool.h"
#include "CRenderable.h"
#include "RenderSystem.h"
#include <GLM\glm.hpp>
#include <vector>

namespace Nova
{

	class GeometryPass;

	class Scene
	{

	/*variables*/
	//-------------------------------------------------------//
	private:
	
		std::vector<Camera> m_cams;
		unsigned int        m_activeCam;
		GLTexture*          m_skytexture;
		bool                m_initialised = false;


	protected:
		StaticPool<ECS::Component::CTransform>  m_transformComps;
		StaticPool<ECS::Component::CRenderable> m_renderComps;
		ECS::RenderSystem                       m_renderSystem;
	//-------------------------------------------------------//

	/*functions*/
	//-------------------------------------------------------//
	private:

		inline void SetActiveCamera(unsigned int i) { m_activeCam = i; }

	protected:

		inline void SetInitialised(bool value) { m_initialised = value; }
		inline bool GetInitialised() const 	{ return m_initialised;	}

		inline void SetSkyboxCubeMap(GLTexture* skybox)
		{
			if (skybox->type != GL_TEXTURE_CUBE_MAP)
			{
				Logger::GetInstance().ErrorBlock("Scene Error", "the skybox could not be set at the texture was not a cubemap", true);
				return;
			}
			m_skytexture = skybox;
		}

	public:
		Scene
			(
			GeometryPass& gpass,
			GLuint        maxTransforms,
			GLuint        maxRenderComps
			)
			:
			m_transformComps(maxTransforms),
			m_renderComps(maxRenderComps),
			m_renderSystem(gpass, m_transformComps, m_renderComps)
		{
			m_cams.push_back(Camera(glm::vec3(),60,Window::GetAspectRatio(),0.0001f,10000.0f));
			m_activeCam = 0;

			////// cubemap //////
			std::string redsky = "Textures/Skyboxes/redsky/";

			ResourceManager::CacheCubeMap("redsky",
				redsky + "r.png",
				redsky + "l.png",
				redsky + "up.png", // top of sphere
				redsky + "bot.png", // bottom of sphere
				redsky + "front.png",
				redsky + "back.png");

			SetSkyboxCubeMap(ResourceManager::GetCubeMap("redsky"));
		}

		~Scene()
		{

		}

		virtual void Initialise() = 0;

		void ub(){

		}

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
		//-------------------------------------------------------//
	};

}

