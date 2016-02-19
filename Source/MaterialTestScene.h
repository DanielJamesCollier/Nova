#pragma once
#include "Scene.h"
#include "Transform.h"

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
			Transform m_transform;
		};

	}
}

