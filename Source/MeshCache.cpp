#include "MeshCache.h"
#include "IOManager.h"
#include "Logger.h"
#include "Mesh.h"

namespace Nova
{
	MeshCache::MeshCache()
		:
		m_meshLoader(MeshLoader::GetInstance())
	{
	
	}


	MeshCache::~MeshCache()
	{
	}

	void MeshCache::Init()
	{
	
	}

	void MeshCache::Dispose()
	{
		DisposeMeshCache();
		DisposeIndexMeshCache();
	}

	void MeshCache::DisposeMeshCache()
	{
		for (auto i = m_meshCache.begin(); i != m_meshCache.end(); i++)
		{
			delete i->second;
		}
		m_meshCache.clear();
	}

	void MeshCache::DisposeIndexMeshCache()
	{
		for (auto i = m_indexedMeshCache.begin(); i != m_indexedMeshCache.end(); i++)
		{
			delete i->second;
		}
		m_indexedMeshCache.clear();
	}

	void MeshCache::DisposeMeshCache(const std::string& name)
	{
		auto search = m_meshCache.find(name);

		if (search != m_meshCache.end())
		{
			delete search->second;
			m_meshCache.erase(search);
		}
		else
		{
			Logger::GetInstance().ErrorBlock("Mesh Cache Error", "Cache: MeshCache\nError: the mesh you are trying to remove is not in the cache\nMesh: " + name, true);
		}
	}

	void MeshCache::DisposeIndexMeshCache(const std::string& name)
	{
		auto search = m_indexedMeshCache.find(name);

		if (search != m_indexedMeshCache.end())
		{
			search->second->DisposeGLResources();
			delete search->second;
			m_indexedMeshCache.erase(search);
		}
		else
		{
			Logger::Logger::GetInstance().ErrorBlock("Mesh Cache Error", "Cache: IndexMeshCache\nError: the mesh you are trying to remove is not in the cache\nMesh: " + name, true);
		}
	}

	void MeshCache::PrintCacheContent()
	{

	}

	void  MeshCache::CacheMesh(const std::string& filepath, Mesh* mesh)
	{
		auto search = m_meshCache.find(filepath);

		if (search != m_meshCache.end())
		{
			Logger::GetInstance().ErrorBlock("Mesh Cache Error", "Error: a mesh already exists in the cache with that name\nName: " + filepath, true);
		}
		else
		{
			if (mesh)
			{
				m_meshCache.emplace(filepath, mesh);
			}
		}
	}

	IndexedMesh* MeshCache::GetIndexedMesh(const std::string& filePath)
	{
		auto search = m_indexedMeshCache.find(filePath);

		if (search == m_indexedMeshCache.end())
		{
			if (IOManager::DoesFileExist(filePath))
			{
				IndexedMesh* mesh = m_meshLoader.LoadIndexedMesh_heap(filePath);

				if (mesh)
				{
					m_indexedMeshCache.emplace(filePath, mesh);
					return mesh;
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				return nullptr;
			}
		}

		return search->second;
	}

	void MeshCache::CacheIndexedMesh(const std::string& name, const std::string& filePath)
	{
		auto search = m_indexedMeshCache.find(filePath);

		if (search != m_indexedMeshCache.end())
		{
			Logger::GetInstance().ErrorBlock("Mesh Cache Error", "Error: a mesh already exists in the cache with that name\nName: " + filePath, true);
		}
		else
		{
			if (IOManager::DoesFileExist(filePath))
			{
				IndexedMesh* mesh = m_meshLoader.LoadIndexedMesh_heap(filePath);
				if (mesh)
				{
					m_indexedMeshCache.emplace(name, mesh);
				}
				else
				{
					Logger::GetInstance().ErrorBlock("Mesh Cache Error", "Error: the mesh could not be added to the cache because it didnt load properly", true);
				}
			}
		}
	}

	void MeshCache::CacheIndexedMesh(const std::string& filepath)
	{
		auto search = m_indexedMeshCache.find(filepath);

		if (search != m_indexedMeshCache.end())
		{
			Logger::GetInstance().ErrorBlock("Mesh Cache Error", "Error: a mesh already exists in the cache with that name\nName: " + filepath, true);
		}
		else
		{
			if (IOManager::DoesFileExist(filepath))
			{
				IndexedMesh* mesh = m_meshLoader.LoadIndexedMesh_heap(filepath);
				if (mesh) 
				{
					m_indexedMeshCache.emplace(filepath, mesh);
				}
				else
				{
					Logger::GetInstance().ErrorBlock("Mesh Cache Error", "Error: the mesh could not be added to the cache because it didnt load properly",true);
				}
			}
		}
	}

	void MeshCache::CacheIndexedMesh(const std::string& filePath, IndexedMesh* indexedMesh)
	{
		if (!indexedMesh) return;
		m_indexedMeshCache.emplace(filePath, indexedMesh);
	}

}
