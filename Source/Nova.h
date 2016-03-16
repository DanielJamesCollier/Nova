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


		SceneManager m_sceneManager;


		bool m_running = false;
		bool m_displayVolumes = false;
		bool m_srgb = false;
		bool m_forwardRender = false;
		bool m_disposed = false;

		
	public:
		explicit Nova(Window window);
		~Nova();
		void Dispose();

		void Start();
		void Stop();
		void Run();

		void CacheResources();
		void CacheMeshes();

		void ProcessInput();
		void Update(float delta);
		void Render();

		// temp functions
		void SetUpLights();		
	};
}
