#include "Window.h"
#include "InputManager.h"

#include "GLEW\glew.h"

#include "Vertex.h"
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Lights.h"
#include "SceneManager.h"
#include "ProfileTimer.h"

#include "Material.h"
#include <vector>

#define pLightCount 500
#define sLightCount 1
#define dLightCount 1

// new deferred renderer
#include "GeometryPass.h"
#include "StencilPass.h"
#include "PointLightPass.h"
#include "SkyBoxPass.h"
#include "DirectionalLightPass.h"
#include "SpotLightPass.h"
#include "GBuffer.h"

namespace Nova
{
	class Nova
	{
	public:
		explicit Nova(Window* window);
		~Nova();

		void Start();
		void Stop();
		void Run();

		void CacheResources();
		void CacheMeshes();
		void CacheTextures();

		void ProcessInput();
		void Update(float delta);
		void Render();

		void DSRenderScene();
		void DSGeometryPass();
		void DSStencilPass(unsigned int i);
		void DSPointLightPass(unsigned int i);
		void DSDirectionalLightPass();
		void DSSpotLightPass();
		void DSSkyboxPass();
		void DSFinalPass();

		void ForwardRender();

		void InitFullScreenQuad();

		// temp functions
		void SetUpLights();
		

	private:

		Window*			m_window;
		InputManager	m_inputManager;

		Transform transform;

		SceneManager m_sceneManager;

		PointLight pLights[pLightCount];
		SpotLight  sLights[sLightCount];
		DirectionalLight dLights[dLightCount];
		GBuffer    m_gBuffer;

		// full screen quad
		GLuint m_vaoQuad;

		bool m_running = false;
		bool m_displayVolumes = false;
		bool m_srgb = false;
		bool m_forwardRender = false;

		float j = 0; // used for test animation

		//deferred renderer
		GeometryPass		 m_geometryPass;
		StencilPass			 m_stencilPass;
		PointLightPass		 m_pointLightPass;
		DirectionalLightPass m_dirLightPass;
		SpotLightPass		 m_spotLightPass;
		SkyBoxPass			 m_skyboxPass;
		IndexedMesh*		 m_sphere; // used for point light stencil buffer and debug point light drawing
	};
}