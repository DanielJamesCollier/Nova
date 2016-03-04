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
#include <iostream>
#include <array>
#include <vector>	
#include <tuple>
#include <unordered_map>

#define MAX_ENTITIES 1000
#define MAX_MESHES 100
#define MAX_MATERIALS 100

namespace Nova
{
	namespace ECS
	{

		struct RenderBatch
		{
		public:
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
				for (unsigned int i = 0, size = m_mesh.GetPoolSize(); i < size; ++i)
				{
					IndexedMesh& mesh = m_mesh.GetObject(i, valid);

					if (valid) mesh.DisposeGLResources();
				}

				for (unsigned int i = 0, size = m_material.GetPoolSize(); i < size; ++i)
				{
					Material& mat = m_material.GetObject(i, valid);

					if (valid) mat.DisposeGLResources();
				}
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
			

				//OldUpdate();
			}


		private:

			void BuildBatches()
			{
				static const unsigned int r_poolSize = m_cRenderable.GetPoolSize();
				std::unordered_map<unsigned int, unsigned int> m_meshToIndex;
				unsigned int index = 0;

				bool valid;

				for (unsigned int i = 0; i < r_poolSize; ++i)
				{
					const Component::CTransform& tComp = m_cTransforms.GetObject(i, valid);
					const Component::CRenderable& rComp = m_cRenderable.GetObject(i, valid);

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
				static const unsigned int r_poolSize = m_batches.size();
				//std::cout << "number of batches: " << r_poolSize << std::endl;
				for (unsigned int i = 0; i < r_poolSize; ++i)
				{


					bool valid;

					IndexedMesh& mesh = m_mesh.GetObject(i, valid);
					Material& material = m_material.GetObject(m_batches[i].material, valid);

					material.Bind(0);

					mesh.DrawBegin();
					{
						const unsigned int r_numBatchItems = m_batches[i].transform.size();

						for (unsigned int j = 0; j < r_numBatchItems; ++j)
						{
							//
							Component::CTransform& trans = m_batches[i].transform[j];
							glm::mat4 model = trans.GetModel();
							glm::mat4 mvp = trans.GetMVP(m_camera->GetViewProject());

							m_gPass.SetModel(model);
							m_gPass.SetMVP(mvp);

							mesh.Draw();
						}

					}
					mesh.DrawEnd();
				}
				m_batches.clear();
			}


			void OldUpdate()
			{
				static const unsigned int r_poolSize = m_cRenderable.GetPoolSize();
				bool valid;

				for (unsigned int i = 0; i < r_poolSize; ++i)
				{
					Component::CTransform& tComp = m_cTransforms.GetObject(i, valid);
					Component::CRenderable& rComp = m_cRenderable.GetObject(i, valid);

					if (valid)
					{
						Material&     mat = m_material.GetObject(rComp.material, valid);
						IndexedMesh& mesh = m_mesh.GetObject(rComp.mesh, valid);

						m_gPass.SetModel(tComp.GetModel());
						m_gPass.SetMVP(tComp.GetMVP(m_camera->GetViewProject()));

						if (valid)
						{
							if (rComp.material != m_currentMat)
							{
								mat.Bind(0);
								m_currentMat = rComp.material;
							}
							mesh.Draw();
						}
					}
				}
			}
			
			StaticPool<Component::CTransform>&       m_cTransforms;
			StaticPool<Component::CRenderable>&      m_cRenderable;
			StaticPool<IndexedMesh>       m_mesh;
			StaticPool<Material>          m_material;
			std::vector<RenderBatch>      m_batches;
		
			GeometryPass& m_gPass;
			Camera*       m_camera;
			MeshLoader&   m_meshLoader;
			int           m_currentMat;



			
		};
	}
}