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
				m_material(100),
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
				std::cout << id << std::endl;
				return id;
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
					const CTransform& tComp = m_cTransforms.GetObject(i, valid);
					const CRenderableTest& rComp = m_cRenderable.GetObject(i, valid);
						if (valid) // remove this when all entities with a cRenderable have to have a transform
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

		private:
			
			StaticPool<CTransform>&       m_cTransforms;
			StaticPool<CRenderableTest>&  m_cRenderable;
			StaticPool<IndexedMesh>       m_mesh;
			StaticPool<Material>          m_material;
		
			GeometryPass& m_gPass;
			Camera*       m_camera;
			MeshLoader&   m_meshLoader;
			int           m_currentMat;

			
		};

		class RenderBatch
		{
			unsigned int mesh_id;
			std::vector<CTransform&> transforms;
		};
	}
}