#pragma once
#include "Scene.h"
#include "CTransform.h"

namespace Nova
{
	namespace ScratchPad
	{
		class MaterialTestScene : public Scene
		{
		public:
			MaterialTestScene();
			~MaterialTestScene();

			void Initialise() override;

			void Update(float delta)		 override;
			void Render(GeometryPass* gPass) override;

		private:
			ECS::Component::CTransform m_transform;
		};

	}
}

