#pragma once
#include "CacheTemplate.h"
#include <unordered_map>
#include <string>

namespace Nova
{
	class IndexedMesh;
	class Mesh;

	class MeshCache
	{
	public:
		MeshCache();
		~MeshCache();

		void Init();
		void Dispose();
		void DisposeMeshCache();
		void DisposeIndexMeshCache();
		void DisposeMeshCache(const std::string& name);
		void DisposeIndexMeshCache(const std::string& name);
		void PrintCacheContent();

		void  CacheMesh(const std::string& filePath, Mesh* mesh);

		IndexedMesh* GetIndexedMesh(const std::string& name);
		void		 CacheIndexedMesh(const std::string& filePath);
		void		 CacheIndexedMesh(const std::string& name, const std::string& filePath);
		void		 CacheIndexedMesh(const std::string& name, IndexedMesh* indexedMesh);

	private:
		std::unordered_map<std::string, Mesh*>		  m_meshCache;
		std::unordered_map<std::string, IndexedMesh*> m_indexedMeshCache;
	};


}
