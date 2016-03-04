#pragma once
#include "Mesh.h"
#include "Material.h"
namespace Nova
{
	namespace ECS
	{
		namespace Component
		{
			struct CRenderable
			{
				unsigned int mesh;
				unsigned int material;
			};
		}	
	}
}