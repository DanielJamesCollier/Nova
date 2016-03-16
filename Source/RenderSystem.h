#pragma once
#include "System.h"
#include "StaticPool.h"
#include "CTransform.h"
#include "CRenderable.h"
#include "Mesh.h"
#include "Material.h"
#include "GeometryPass.h"
#include "Camera.h"
#include "MeshLoader.h"
#include "CSpotLight.h"
#include "CPointLight.h"
#include "CDirectionalLight.h"
#include "TextureBinder.h"
#include "Utils.h"
#include "ResourceManager.h"
#include "Sphere.h"

#include <iostream>
#include <array>
#include <vector>	
#include <tuple>
#include <unordered_map>

//deferred rendering includes
#include "GBuffer.h"
#include "GeometryPass.h"
#include "StencilPass.h"
#include "PointLightPass.h"
#include "SpotLightPass.h"
#include "DirectionalLightPass.h"
#include "SkyBoxPass.h"

#include "Lights.h"

#define MAX_MESHES       100
#define MAX_MATERIALS    100
#define MAX_LIGHTS_POINT 200
#define MAX_LIGHTS_SPOT  100

namespace Nova
{
	namespace ECS
	{

		struct RenderBatch
		{
			std::vector<Component::CTransform> transform;
			unsigned int            material;
			unsigned int			mesh;
		};

		class RenderSystem : public System
		{
		private:

			/* components */
			//-------------------/
			StaticPool<Component::CTransform>&        m_cTransforms;
			StaticPool<Component::CRenderable>&       m_cRenderable;
			StaticPool<Component::Light::CPointLight> m_pointLights;
			StaticPool<Component::Light::CSpotLight>  m_spotLights;
			Component::Light::CDirectionalLight       m_directionalLight;
			//-------------------/

			/*Deferred Renderer*/
			//------------------//
			GBuffer              m_gBuffer;
			GeometryPass		 m_geometryPass;
			StencilPass			 m_stencilPass;
			PointLightPass		 m_pointLightPass;
			DirectionalLightPass m_dirLightPass;
			SpotLightPass		 m_spotLightPass;
			SkyBoxPass			 m_skyboxPass;
			//------------------//

			/* Data Pools*/
			//------------------//
			StaticPool<IndexedMesh>					 m_mesh;
			StaticPool<Material>					 m_material;
			//------------------//

			/* Lights */
			//------------------//
			PointLight      pLights[MAX_LIGHTS_POINT];
			SpotLight       sLights[MAX_LIGHTS_SPOT];
			DirectionalLight dLight;
			//------------------//

			// batches
			std::vector<RenderBatch>				 m_batches;

			// data
			Camera*       m_camera;
			MeshLoader&   m_meshLoader;
			int           m_currentMat;

			IndexedMesh*  m_pointLightStencilVolume;
			IndexedMesh*  m_skyboxCube;
			GLTexture*    m_skyboxTexture;

			ECS::Component::CTransform m_transform;

			// full screen quad
			GLuint m_vaoQuad;

		public:
			RenderSystem(StaticPool<Component::CTransform>& transformComps, StaticPool<Component::CRenderable>& renderComps);
			~RenderSystem();

			void InitialiseDSPasses();

			void InitFullScreenQuad();

			int AddMesh(const IndexedMesh& mesh);
			
			int AddMesh(const std::string& filePath);

			int AddMaterial(const Material& material);

			void SetCamera(Camera* camera);

			void Update() override;

			void SetSkyboxTexture(GLTexture* texture);

		private:

			/*Deferred Renderer*/
			//------------------//
			void DSGeometryPass();
			void DSBuildBatches();
			void DSDrawBatches();
			void DSPointLightStencilPass(unsigned int i);
			void DSDirectionalLightPass();
			void DSSkyboxPass();
			void DSFinalPass();
			//------------------//

		
		};
	}
}