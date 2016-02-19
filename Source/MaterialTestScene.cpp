#include "MaterialTestScene.h"
#include "GeometryPass.h"

namespace Nova
{
	namespace ScratchPad
	{
		MaterialTestScene::MaterialTestScene()
		{
		}

		MaterialTestScene::~MaterialTestScene()
		{
		}

		void MaterialTestScene::Initialise()
		{
			if (GetInitialised()) return;
			SetInitialised(true);

			////// cubemap //////
			std::string redsky = "Textures/Skyboxes/redsky/";

			ResourceManager::CacheCubeMap("redsky",
				redsky + "r.png",
				redsky + "l.png",
				redsky + "up.png", // top of sphere
				redsky + "bot.png", // bottom of sphere
				redsky + "front.png",
				redsky + "back.png");
			////////////////

			SetSkyboxCubeMap(ResourceManager::GetCubeMap("redsky"));
		}

		void MaterialTestScene::Update(float delta)
		{
			UpdateActiveCamera(delta);
		}

		void MaterialTestScene::Render(GeometryPass* gPass)
		{

		}
	}
}