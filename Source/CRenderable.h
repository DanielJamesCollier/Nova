#pragma once
#include "Mesh.h"
#include "Material.h"
namespace Nova
{
	namespace ECS
	{
		// final cRenderable that will use batching - TODO
		struct CRenderable
		{
			CRenderable(int mesh, int material)
			{
				meshIndex = mesh;
				materialIndex = material;
			}

			int meshIndex;
			int materialIndex;
		};

		// CRenderable with indirection
		struct CRenderableTest
		{
			unsigned int mesh;
			Material* material;
		};
	}
}