#pragma once

#include "TextureCache.h"
#include "ShaderProgramCache.h"
#include "ShaderOBJCache.h"
#include "MeshCache.h"
#include "MaterialCache.h"
#include <string>


/*TODO add selective cache functionality - give the programmer the option weather he wants to cache or not */



namespace Nova
{
	class ResourceManager
	{
	public:
		ResourceManager() = delete;
		~ResourceManager() = delete;

		static void Init();
		static void Dispose();



		/*--------------Shaders-------------*/
		//////////////////////////////////////
		/*---Shader Program Cache---*/
		static ShaderProgram*	  GetShaderProgram(const std::string& name);
		static void				  CacheShaderProgram(ShaderProgram* program);
		static void				  DisposeShaderProgram(const std::string& name);
		static void				  DisposeShaderProgramCache();
		static const std::string& GetShaderProgramCacheInfo();

		/*---Shader Object Cache---*/
		static ShaderOBJ*		  GetShaderOBJ(const std::string& name);
		static void				  CacheShaderOBJ(const std::string& objectFilePath);
		static void				  CacheShaderOBJ(const std::string& name, const std::string& objectFilePath);
		static void				  DisposeShaderOBJ(const std::string& name);
		static void				  DisposeShaderOBJCache();
		static const std::string& GetShaderObjectCacheInfo();
		//////////////////////////////////////


		/*-------------Textures------------*/
		/////////////////////////////////////
		/*---Texture Cache---*/
		static void			      CacheTexture(const std::string& texturePath);
		static void				  CacheTexture(const std::string& name, const std::string& texturePath);
		static void				  CacheTexture(const std::string& name, GLTexture* texture);
		static GLTexture*         GetTexture(const std::string& name);
		static void			      DisposeTexture(const std::string& name);
		static void				  DisposeTextureCache();
		static const std::string& GetTextureCacheInfo();
		static void				  CacheCubeMap(const std::string& name, const std::string& posX, const std::string& negX, const std::string& posY, const std::string& negY, const std::string& posZ, const std::string& negZ);
		static void				  CacheCubeMap(const std::string& name, GLTexture* texture);
		static GLTexture*		  GetCubeMap(const std::string& name);
		static void				  DisposeCubeMap(const std::string& name);
		static void				  DisposeCubeMaps();
		static const std::string& GetCubeMapCacheInfo();
		/////////////////////////////////////


		/*-------------Meshes------------*/
		/////////////////////////////////////
		/*---Mesh Cache---*/
		static void				 CacheMesh(const std::string& meshPath, Mesh* mesh);
		static void				 DisposeMesh(const std::string& meshPath);
		static void				 DisposeMeshCache();
		static const std::string GetMeshCacheInfo();

		/*---Indexed Mesh Cache---*/
		static void				 CacheIndexedMesh(const std::string& meshPath);
		static void				 CacheIndexedMesh(const std::string& name, const std::string& meshPath);
		static void				 CacheIndexedMesh(const std::string& name, IndexedMesh* mesh);
		static IndexedMesh*		 GetIndexedMesh(const std::string& name);
		static void				 DisposeIndexedMesh(const std::string& name);
		static void				 DisposeIndexedMeshCache();
		static const std::string GetIndexedMeshCacheInfo();
		/*---All---*/
		static void			 DisposeAllMeshCaches();
		/////////////////////////////////////


		/*-------------Materials------------*/
		/////////////////////////////////////
		static void				 DisposeMaterial(const std::string& name);
		static void				 DisposeMaterialCache();
		static void				 AddMaterial(const std::string& name, Material* material);
		static Material*		 GetMaterial(const std::string& name);
		static const std::string GetMaterialsCacheInfo();
		/////////////////////////////////////

	private:
		static TextureCache       m_TextureCache;
		static ShaderProgramCache m_shaderProgramCache;
		static ShaderOBJCache	  m_shaderOBJCache;
		static MeshCache		  m_meshCache;
		static MaterialCache	  m_materialCache;
	};

}



