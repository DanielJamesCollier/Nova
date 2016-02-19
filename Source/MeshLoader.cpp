#include "MeshLoader.h"
#include "Logger.h"
#include <ASSIMP\Importer.hpp>
#include <ASSIMP\scene.h>
#include <ASSIMP\postprocess.h>
#include <vector>
#include "Vertex.h"
#include "Mesh.h"
#include <iostream>

namespace Nova
{
	MeshLoader::MeshLoader()
	{
	}


	MeshLoader::~MeshLoader()
	{
	}

	IndexedMesh* MeshLoader::LoadIndexedMesh(const std::string& filePath)
	{

		Assimp::Importer importer;

		unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_ImproveCacheLocality | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeMeshes;


		Logger::InfoBlockBegin("Mesh Loader");
		Logger::InfoBlockMessage("File: " + filePath);

		Logger::InfoBlockMessage("\nStarting import...");
		const aiScene* scene = importer.ReadFile(filePath, flags);




		if (!scene)
		{
			Logger::ErrorBlock("Mesh Loader Error", "Error: could not load model", true);
			return nullptr;
		}


		if (scene->HasMeshes())
		{
			std::vector<Vertex>       vertices;
			std::vector<unsigned int> indices;

			unsigned int indicesOffset = 0; // used for mesh merging

			for (unsigned int ii = 0; ii < scene->mNumMeshes; ii++)
			{
				aiMesh* mesh = scene->mMeshes[ii];

				if (!mesh)
				{
					Logger::ErrorBlock("Mesh Loader Error", "Error: no model was found in the file\nFile: " + filePath, true);
				}

				if (!IsMeshValid(mesh, ii))
				{
					std::cout << "the mesh was corrupt" << std::endl;
					return nullptr;
				}

				// get vertices normals and tex coords
				for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
				{

					aiVector3D &pos = mesh->mVertices[i];
					aiVector3D &norm = mesh->mNormals[i];
					aiVector3D tex = tex = mesh->mTextureCoords[0][i];
					aiVector3D tan = aiVector3D(0, 0, 0);


					if (mesh->HasTangentsAndBitangents())
					{
						tan = mesh->mTangents[i];
					}


					glm::vec3 glm_position = glm::vec3(pos.x, pos.y, pos.z);
					glm::vec3 glm_tangent = glm::vec3(tan.x, tan.y, tan.z);
					glm::vec2 glm_uvCoords = glm::vec2(tex.x, tex.y);
					glm::vec3 glm_normals = glm::vec3(norm.x, norm.y, norm.z);
					glm::vec4 glm_colour = glm::vec4(1.0, 1.0, 1.0, 1.0);

					vertices.push_back(Vertex(glm_position, glm_colour, glm_uvCoords, glm_normals, glm_tangent));
				}

				// get indices
				for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
				{
					aiFace face = mesh->mFaces[i];
					for (GLuint j = 0; j < face.mNumIndices; j++)
						indices.push_back(face.mIndices[j] + indicesOffset);
				}

				indicesOffset += mesh->mNumVertices;

			}
			Logger::InfoBlockMessage("\nImport complete");
			Logger::InfoBlockMessage("\nVertex Count: " + std::to_string(vertices.size()));
			Logger::InfoBlockMessage("\nElement Count " + std::to_string(indices.size()));
			Logger::InfoBlockEnd();


			return new IndexedMesh(vertices, indices);
		}

		return nullptr;
	}

	bool MeshLoader::IsMeshValid(aiMesh* mesh, unsigned int pIndex)
	{
		if (mesh->HasPositions())
		{
			
			std::cout << "\nPositions: valid" << std::endl;
		}
		else
		{
			std::cout << "Positions: invalid" << std::endl;
			return false;
		}

		if (mesh->HasNormals())
		{
			std::cout << "Normals: valid" << std::endl;
		}
		else
		{
			std::cout << "Normals: invalid" << std::endl;
			return false;
		}

		if (mesh->HasVertexColors(pIndex))
		{
			std::cout << "Vertex Colours: valid" << std::endl;
		}
		else
		{
			std::cout << "Vertex Colours: invalid" << std::endl;
		}

		if (mesh->HasBones())
		{
			std::cout << "Bones: valid" << std::endl;
		}
		else
		{
			std::cout << "Bones: invalid" << std::endl;
		}

		if (mesh->HasTangentsAndBitangents())
		{
			std::cout << "Tangents_Bitangents: valid" << std::endl;
		}
		else
		{
			std::cout << "Tangents_Bitangents: invalid" << std::endl;
		}

		if (mesh->HasTextureCoords(pIndex))
		{
			std::cout << "Texture Coordinates: valid" << std::endl;	
		}
		else
		{
			std::cout << "Texture Coordinates: invalid" << std::endl;
		}

		return true;
	}

}
