#pragma once
#include <string>
#include <ASSIMP\scene.h>
namespace Nova
{
	class IndexedMesh;

	class MeshLoader
	{
	public:
		MeshLoader();
		~MeshLoader();

		static IndexedMesh* LoadIndexedMesh(const std::string& filePath);

	private:
		static bool IsMeshValid(aiMesh* mesh, unsigned int pIndex);
	};


}
