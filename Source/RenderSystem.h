#pragma once
#include "System.h"
#include "StaticPool.h"
#include "CTransform.h"
#include "CRenderable.h"
#include "Mesh.h"
#include "Material.h"
#include "GeometryPass.h"
#include "Camera.h"
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
				//m_mesh(100),
				//m_material(100),
				m_cTransforms(transformComps),
				m_cRenderable(renderComps),
				m_gPass(gPass)
			{

			}

			~RenderSystem()
			{

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
					CRenderableTest& rComp = m_cRenderable.GetObject(i, valid);
					if (valid)
					{
						CTransform& tComp = m_cTransforms.GetObject(i, valid);
						if (valid) // remove this when all entities with a cRenderable have to have a transform
						{
							//Material&     mat = m_material.GetObject(rComp.materialIndex, valid);
							//IndexedMesh& mesh = m_mesh.GetObject(rComp.meshIndex, valid);

							if (valid) // remove this when a default material and mesh is added.
							{
								rComp.material->Bind(0);
								m_gPass.SetModel(tComp.GetModel());
								m_gPass.SetMVP(tComp.GetMVP(m_camera->GetViewProject()));
								rComp.mesh->Draw();
							}
						}
					}
				}
			}

		private:
			StaticPool<CTransform>&       m_cTransforms;
			StaticPool<CRenderableTest>&  m_cRenderable;
		//	StaticPool<IndexedMesh>   m_mesh;
		//	StaticPool<Material>	  m_material;
			GeometryPass& m_gPass;
			Camera* m_camera;
		};
	}
}