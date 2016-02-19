#include "ResourceManager.h"
#include "GLTexture.h"
#include "ShaderProgram.h"
#include "IOManager.h"

#include <iostream>	
#include <fstream>

namespace Nova
{

	TextureCache       ResourceManager::m_TextureCache;
	ShaderProgramCache ResourceManager::m_shaderProgramCache;
	ShaderOBJCache     ResourceManager::m_shaderOBJCache;
	MeshCache		   ResourceManager::m_meshCache;
	MaterialCache      ResourceManager::m_materialCache;
	

	//TODO sort out initial resources
	void ResourceManager::Init()
	{

		/*check to see if there is a startup folder with .ini files if not create one*/
		if (IOManager::DoesFileExist("Startup/shaders.ini"))
		{
			// read from the file
		}
		else
		{
			// create the file
			std::ofstream file("Startup/Shaders.ini");
		}

		m_TextureCache.Init();
		m_shaderProgramCache.Init();
		m_shaderOBJCache.Init();
		m_meshCache.Init();
		m_materialCache.Init();
	}

	void ResourceManager::Dispose()
	{
		m_TextureCache.Dispose();
		m_shaderProgramCache.Dispose();
		m_shaderOBJCache.Dispose();
		m_meshCache.Dispose();
		m_materialCache.Dispose();
	}

	void ResourceManager::CacheTexture(const std::string& texturePath)
	{
		m_TextureCache.CacheTexture(texturePath);
	}

	void ResourceManager::CacheTexture(const std::string& name, const std::string& texturePath)
	{
		m_TextureCache.CacheTexture(name, texturePath);
	}

	void ResourceManager::CacheTexture(const std::string& name, GLTexture* texture)
	{
		m_TextureCache.CacheTexture(name, texture);
	}

	GLTexture* ResourceManager::GetTexture(const std::string& name)
	{
		return m_TextureCache.GetTexture(name);
	}

	void ResourceManager::DisposeTexture(const std::string& name)
	{
		m_TextureCache.Dispose(name);
	}

	void ResourceManager::DisposeTextureCache()
	{
		m_TextureCache.Dispose();
	}

	void  ResourceManager::CacheCubeMap(const std::string& name, const std::string& posX, const std::string& negX, const std::string& posY, const std::string& negY, const std::string& posZ, const std::string& negZ)
	{
		m_TextureCache.CacheCubeMap(name, posX, negX, posY, negY, posZ, negZ);
	}

	void  ResourceManager::CacheCubeMap(const std::string& name, GLTexture* texture)
	{
		m_TextureCache.CacheCubeMap(name, texture);
	}

	GLTexture* ResourceManager::GetCubeMap(const std::string& name)
	{
		return m_TextureCache.GetCubeMap(name);
	}

	void ResourceManager::DisposeCubeMap(const std::string& name)
	{
		m_TextureCache.DisposeCubeMap(name);
	}

	void ResourceManager::DisposeCubeMaps()
	{
		m_TextureCache.DisposeCubeMaps();
	}

	void ResourceManager::CacheShaderProgram(ShaderProgram* program)
	{
		m_shaderProgramCache.CacheProgram(program);
	}

	ShaderProgram* ResourceManager::GetShaderProgram(const std::string& name)
	{
		return m_shaderProgramCache.GetProgram(name);
	}

	void ResourceManager::DisposeShaderProgram(const std::string& name)
	{
		m_shaderProgramCache.Dispose(name);
	}

	void ResourceManager::DisposeShaderProgramCache()
	{
		m_shaderProgramCache.Dispose();
	}

	ShaderOBJ* ResourceManager::GetShaderOBJ(const std::string& name)
	{
		return m_shaderOBJCache.GetShaderOBJ(name);
	}

	void ResourceManager::CacheShaderOBJ(const std::string& objectFilePath)
	{
		m_shaderOBJCache.CacheShaderOBJ(objectFilePath);
	}

	void ResourceManager::CacheShaderOBJ(const std::string& name, const std::string& objectFilePath)
	{
		m_shaderOBJCache.CacheShaderOBJ(name, objectFilePath);
	}

	void ResourceManager::DisposeShaderOBJ(const std::string& objectFilePath)
	{
		m_shaderOBJCache.Dispose(objectFilePath);
	}

	void ResourceManager::DisposeShaderOBJCache()
	{
		m_shaderOBJCache.Dispose();
	}

	void ResourceManager::CacheMesh(const std::string& meshPath, Mesh* mesh)
	{
		m_meshCache.CacheMesh(meshPath, mesh);
	}

	void ResourceManager::DisposeMesh(const std::string& meshPath)
	{
		m_meshCache.DisposeMeshCache(meshPath);
	}

	void ResourceManager::DisposeMeshCache()
	{
		m_meshCache.Dispose();
	}

	void ResourceManager::CacheIndexedMesh(const std::string& meshPath)
	{
		m_meshCache.CacheIndexedMesh(meshPath);
	}

	void ResourceManager::CacheIndexedMesh(const std::string& name, const std::string& meshPath)
	{
		m_meshCache.CacheIndexedMesh(name,meshPath);
	}

	void ResourceManager::CacheIndexedMesh(const std::string& name, IndexedMesh* mesh)
	{
		m_meshCache.CacheIndexedMesh(name, mesh);
	}

	IndexedMesh* ResourceManager::GetIndexedMesh(const std::string& meshPath)
	{
		return m_meshCache.GetIndexedMesh(meshPath);
	}

	void ResourceManager::DisposeIndexedMesh(const std::string& meshPath)
	{
		m_meshCache.DisposeIndexMeshCache(meshPath);
	}

	void ResourceManager::DisposeIndexedMeshCache()
	{
		m_meshCache.DisposeIndexMeshCache();
	}

	void ResourceManager::DisposeAllMeshCaches()
	{
		m_meshCache.Dispose();
	}

	void ResourceManager::DisposeMaterial(const std::string& name)
	{
		m_materialCache.Dispose(name);
	}

	void ResourceManager::DisposeMaterialCache()
	{
		m_materialCache.Dispose();
	}

	void ResourceManager::AddMaterial(const std::string& name, Material* material)
	{
		m_materialCache.AddMaterial(name, material);
	}

	Material* ResourceManager::GetMaterial(const std::string& name)
	{
		return m_materialCache.GetMaterial(name);
	}

}


