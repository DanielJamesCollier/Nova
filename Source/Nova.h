#include "Window.h"
#include "InputManager.h"

#include "GLEW\glew.h"

#include "Vertex.h"
#include "Mesh.h"
#include "Camera.h"
#include "Lights.h"
#include "SceneManager.h"
#include "ProfileTimer.h"
#include "CTransform.h"
#include "Material.h"
#include <vector>

#define pLightCount 100
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

		Window			m_window; // important to be here because of construction descruction order
		InputManager	m_inputManager;

		ECS::Component::CTransform m_transform;

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



					 //deferred renderer
		GeometryPass		 m_geometryPass;
		StencilPass			 m_stencilPass;
		PointLightPass		 m_pointLightPass;
		DirectionalLightPass m_dirLightPass;
		SpotLightPass		 m_spotLightPass;
		SkyBoxPass			 m_skyboxPass;
		IndexedMesh*		 m_sphere; // used for point light stencil buffer and debug point light drawing
		
	public:
		explicit Nova(Window window);
		~Nova();

		void Start();
		void Stop();
		void Run();

		void CacheResources();
		void CacheMeshes();

		void ProcessInput();
		void Update(float delta);
		void Render();

		/*Deferred Rendering Functions*/
		/////////////////////
		void DSRenderScene();
		void DSGeometryPass();
		void DSPointLightStencilPass(unsigned int i);
		void DSDirectionalLightPass();
		void DSSpotLightPass();
		void DSSkyboxPass();
		void DSFinalPass();
		////////////////////

		void InitFullScreenQuad();

		// temp functions
		void SetUpLights();		
	};
}
