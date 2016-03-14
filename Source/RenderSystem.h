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
#include <iostream>
#include <array>
#include <vector>	
#include <tuple>
#include <unordered_map>

#define MAX_MESHES       100
#define MAX_MATERIALS    100
#define MAX_LIGHTS_POINT 500
#define MAX_LIGHTS_SPOT  500

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
		public:
			RenderSystem() = delete;
			RenderSystem(GeometryPass& gPass, StaticPool<Component::CTransform>& transformComps, StaticPool<Component::CRenderable>& renderComps)
				:
				m_mesh(MAX_MESHES),
				m_material(MAX_MATERIALS),
				m_pointLights(MAX_LIGHTS_POINT),
				m_spotLights(MAX_LIGHTS_SPOT),
				m_cTransforms(transformComps),
				m_cRenderable(renderComps),
				m_gPass(gPass),
				m_meshLoader(MeshLoader::GetInstance())
			{
				m_currentMat = -1;
			}

			~RenderSystem()
			{
				bool valid;
				for (unsigned int i = 0, size = m_mesh.GetActiveObjectCount(); i < size; ++i)
				{
					IndexedMesh& mesh = m_mesh.GetObjectWithPoolID(i, valid);

					if (valid) mesh.DisposeGLResources();
				}

				for (unsigned int i = 0, size = m_material.GetActiveObjectCount(); i < size; ++i)
				{
					Material& mat = m_material.GetObjectWithPoolID(i, valid);

					if (valid) mat.DisposeGLResources();
				}

				ShaderBinder::UnbindShaderProgram();
			}

			int AddMesh(const IndexedMesh& mesh)
			{
				return m_mesh.AddObject(mesh);
			}
			
			int AddMesh(const std::string& filePath)
			{
				bool valid;

				IndexedMesh mesh = m_meshLoader.LoadIndexedMesh_stack(filePath, valid);
	
				if (valid)
				{
					int id = m_mesh.AddObject(mesh);
					return id;
				}
				return -1;
			}

			int AddMaterial(const Material& material)
			{
				int id = m_material.AddObject(material);
				return id;
			}
				
			void SetCamera(Camera* camera)
			{
				m_camera = camera;
			}

			void Update() override
			{
				BuildBatches();
				DrawBatches();
			}
		
		private:

			void BuildBatches()
			{
				unsigned int r_poolSize = m_cRenderable.GetActiveObjectCount();
				std::unordered_map<unsigned int, unsigned int> m_meshToIndex;
				unsigned int index = 0;

				bool valid = false;

				for (unsigned int i = 0; i < r_poolSize; ++i)
				{
					unsigned int staticID = 0;
					const Component::CRenderable& rComp = m_cRenderable.GetObjectWithPoolID(i, staticID, valid);
					const Component::CTransform& tComp  = m_cTransforms.GetObjectWithStaticID(staticID, valid);
					

					if (valid) // if entity has a transform and a renderable comp process it
					{
						// maps mesh id's to an index location in the m_batches vector
						// used to check if a back aready exist for that mesh

						auto search = m_meshToIndex.find(rComp.mesh);

						// if mesh is not found in a batch then create a new batch and add it
						if (search == m_meshToIndex.end())
						{
							RenderBatch batch;
							batch.transform.push_back(tComp);
							batch.material = rComp.material;
							batch.mesh = rComp.mesh;
							m_batches.push_back(batch);
							m_meshToIndex.insert(std::make_pair(batch.mesh, index));
							index++;
						}
						else // // the mesh was found in a batch - add another transform to that batch
						{
							if (m_batches[search->second].material == rComp.material)
							{
								m_batches[search->second].transform.push_back(tComp);
							}
							else
							{
								RenderBatch batch;
								batch.transform.push_back(tComp);
								batch.material = rComp.material;
								batch.mesh = rComp.mesh;
								m_batches.push_back(batch);
								m_meshToIndex.insert(std::make_pair(batch.mesh, index++));
							}
						}
					}
				}
			}

			void DrawBatches()
			{
				unsigned int r_poolSize = m_batches.size();
				//std::cout << "number of batches: " << r_poolSize << std::endl;
				for (unsigned int i = 0; i < r_poolSize; ++i)
				{
					bool valid;


					IndexedMesh& mesh = m_mesh.GetObjectWithStaticID(m_batches[i].mesh, valid);
					Material& material = m_material.GetObjectWithStaticID(m_batches[i].material, valid);

					TextureBinder::GetInstance().BindTexture(5, material.GetTexture());

					mesh.DrawBegin();
					{
						unsigned int r_numBatchItems = m_batches[i].transform.size();

						for (unsigned int j = 0; j < r_numBatchItems; ++j)
						{
							//
							Component::CTransform& trans = m_batches[i].transform[j];

							m_gPass.SetModel(trans.GetModel());
							m_gPass.SetMVP(trans.GetMVP(m_camera->GetViewProject()));

							mesh.DrawGroup();
						}

					}
					mesh.DrawEnd();
				}
				m_batches.clear();
			}			

			// components
			StaticPool<Component::CTransform>&        m_cTransforms;
			StaticPool<Component::CRenderable>&       m_cRenderable;
			StaticPool<Component::Light::CPointLight> m_pointLights;
			StaticPool<Component::Light::CSpotLight>  m_spotLights;
			Component::Light::CDirectionalLight       m_directionalLight;

			// data pools
			StaticPool<IndexedMesh>					 m_mesh;
			StaticPool<Material>					 m_material;

			// batches
			std::vector<RenderBatch>				 m_batches;
			
			// data
			GeometryPass& m_gPass;
			Camera*       m_camera;
			MeshLoader&   m_meshLoader;
			int           m_currentMat;			
		};
	}
}