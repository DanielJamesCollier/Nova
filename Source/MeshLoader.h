#pragma once
#include <string>
#include <ASSIMP\scene.h>
namespace Nova
{
	class IndexedMesh;

	class MeshLoader
	{
	public:

		MeshLoader(MeshLoader const&) = delete;
		void operator=(MeshLoader&)   = delete;

		static MeshLoader& GetInstance() {
			static MeshLoader instance;
			return instance;
		}

		IndexedMesh* LoadIndexedMesh_heap(const std::string& filePath);
		IndexedMesh  LoadIndexedMesh_stack(const std::string& filePath, bool& valid);

	private:
		MeshLoader() {};

		static bool IsMeshValid(aiMesh* mesh, unsigned int pIndex);	
	};
}
