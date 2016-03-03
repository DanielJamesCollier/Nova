#pragma once
#include "Mesh.h"
#include "Material.h"
namespace Nova
{
	namespace ECS
	{
		// CRenderable with indirection
		struct CRenderable
		{
			unsigned int mesh;
			unsigned int material;
		};
	}
}