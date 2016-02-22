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
namespace Nova
{
	namespace ECS
	{
		class RenderSystem : public System
		{
		public:
			RenderSystem() = delete;
			RenderSystem(GeometryPass& gPass, StaticPool<CTransform>& transformComps, StaticPool<CRenderableTest>& renderComps)
				:
				m_mesh(100),
				//m_material(100),
				m_cTransforms(transformComps),
				m_cRenderable(renderComps),
				m_gPass(gPass),
				m_meshLoader(MeshLoader::GetInstance())
			{
				m_currentMat = nullptr;
			}

			~RenderSystem()
			{

			}
			
			int AddMesh(const std::string& filePath)
			{
				bool valid;

				IndexedMesh mesh = m_meshLoader.LoadIndexedMesh_stack(filePath, valid);
				
				if (valid)
					return m_mesh.AddObject(mesh);
				return -1;
			}
				
			void SetCamera(Camera* camera)
			{
				m_camera = camera;
			}

			void Update() override
			{
				static const unsigned int r_poolSize = m_cRenderable.GetPoolSize();
				bool valid;
		
				for (unsigned int i = 0; i < r_poolSize; ++i)
				{

					//CRenderable& rComp = m_cRenderable.GetObject(i, valid);
					CTransform& tComp = m_cTransforms.GetObject(i, valid);
					CRenderableTest& rComp = m_cRenderable.GetObject(i, valid);
						if (valid) // remove this when all entities with a cRenderable have to have a transform
						{
							//Material&     mat = m_material.GetObject(rComp.materialIndex, valid);
							//IndexedMesh& mesh = m_mesh.GetObject(rComp.meshIndex, valid);

								if (rComp.material != m_currentMat)
								{
									rComp.material->Bind(0);
									m_currentMat = rComp.material;

								}
								//rComp.material->Bind(0);
								m_gPass.SetModel(tComp.GetModel());
								m_gPass.SetMVP(tComp.GetMVP(m_camera->GetViewProject()));
								IndexedMesh& mesh = m_mesh.GetObject(rComp.mesh, valid);

								if (valid)
								{
									mesh.Draw();
									std::cout << "drawing" << std::endl;
								}
							
						
					}
				}
			}

		private:
			StaticPool<CTransform>&       m_cTransforms;
			StaticPool<CRenderableTest>&  m_cRenderable;
			StaticPool<IndexedMesh>       m_mesh;
		
			GeometryPass& m_gPass;
			Camera* m_camera;
			MeshLoader& m_meshLoader;
			// current material
			Material* m_currentMat;
		};

		class RenderBatch
		{

		};
	}
}