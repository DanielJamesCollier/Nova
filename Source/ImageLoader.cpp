 #include "ImageLoader.h"
#include "GLTexture.h"
#include "IOManager.h"
#include "Logger.h"
#include <SOIL/SOIL.h>
#include <STBIMAGE\stb_image.h>

#include <algorithm>

#include <iostream>


namespace Nova
{
	GLTexture* ImageLoader::LoadTexture(const std::string& file_path, unsigned int texture_type, bool genMipmap)
	{

		// if the file doesnt exist return a nullprt
		if (!IOManager::DoesFileExist(file_path)) return nullptr;

		GLTexture* texture = new Nova::GLTexture();
		texture->type = texture_type;

		unsigned char* img = SOIL_load_image(file_path.c_str(), &texture->width, &texture->height, NULL, 0);

		// generate texture
		glGenTextures(1, &(texture->id));

		// bind the generated texture
		glBindTexture(texture_type, texture->id);

		// send the image to the gpu
		glTexImage2D(texture_type, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

		// if the mesh is bigger than the texture the texture will wrap
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameterf(texture_type, GL_TEXTURE_LOD_BIAS, -1);
		if (genMipmap)
		{
			glTexParameterf(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(texture_type);
		}
		else
		{
			glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		//TODO check if capable
		float amount = GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT;

		std::cout << "max: " << amount << std::endl;

		glTexParameterf(texture_type, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);

		// unbind the texture and free data
		glBindTexture(texture_type, 0);
		
		SOIL_free_image_data(img);

		return texture;

	}

	GLTexture* ImageLoader::LoadImageNew(const std::string& filePath, unsigned int textureType, bool genMips)
	{
		// if the file doesnt exist return a nullprt
		if (!IOManager::DoesFileExist(filePath)) return nullptr;

		GLTexture* texture = new GLTexture();
		texture->type = textureType;

		unsigned char* data = stbi_load(filePath.c_str(), &texture->width, &texture->height, &texture->components, 4);

		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		std::cout << "id: " << texture->id << std::endl;

		// send the image to the gpu
		glTexImage2D(textureType, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// if the mesh is bigger than the texture the texture will wrap
		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (genMips)
		{
			glTexParameterf(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(textureType);
		}
		else
		{
			glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		stbi_image_free(data);
		glBindTexture(textureType, 0);
	
	}

	GLTexture* ImageLoader::LoadCubeMap(const std::string& posXFile, const std::string& negXFile, const std::string& posYFile, const std::string& negYFile, const std::string& posZFile, const std::string& negZFile)
	{
		if (!IOManager::DoesFileExist(posXFile) ||
			!IOManager::DoesFileExist(negXFile) ||
			!IOManager::DoesFileExist(posYFile) ||
			!IOManager::DoesFileExist(negYFile) ||
			!IOManager::DoesFileExist(posZFile) ||
			!IOManager::DoesFileExist(negZFile))
		{
			Logger::GetInstance().ErrorBlock("Image Loader Error", "Error: the cubemap you are trying to load contains an invalid file location", true);
			return nullptr;
		}

		std::vector<std::string> fileNames;
		fileNames.push_back(posXFile); fileNames.push_back(negXFile);
		fileNames.push_back(posYFile); fileNames.push_back(negYFile);
		fileNames.push_back(posZFile); fileNames.push_back(negZFile);

		std::vector<GLuint> types;
		types.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_X); types.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
		types.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_Y); types.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
		types.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_Z); types.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

		GLTexture* texture = new Nova::GLTexture();
		texture->type = GL_TEXTURE_CUBE_MAP;

		glGenTextures(1, &texture->id);
		glBindTexture(texture->type, texture->id);

		for (unsigned int i = 0; i < fileNames.size(); i++)
		{
			unsigned char* img = SOIL_load_image(fileNames[i].c_str(), &texture->width, &texture->height, NULL, 0);

			glTexImage2D(types[i], 0, GL_RGB, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			free(img);

		}
		// unbind the texture and free data
		glBindTexture(texture->type, 0);


		return texture;
	}

	GLTexture* ImageLoader::LoadImagesInto2DTexArray(const std::vector<std::string>& imagePaths, bool genMipMaps)
	{
		int width;
		int height;


		// load in the first image and store the - width, height & component for comparison with later images
		// important because array texture layers have to be of the same dimension

		std::vector<unsigned char*> imgData;

		// the first image in the image paths vector defines the dimensions for the texture array
		imgData.push_back(SOIL_load_image(imagePaths[0].c_str(), &width, &height, NULL, 0));
		if (imgData[0] == nullptr)
		{
			Logger::GetInstance().ErrorBlock("Image Loader Error", "File: " + imagePaths[0] + "\nError: the file was not loaded.", true);
			return nullptr;
		}

		int compWidth;
		int compHeight;
		// refrence for theses sent into SOIL_load_image below. they are used to compare the size of the images again
		// the first loaded image to make sure they all have the same dimensions and component count
		for (unsigned int i = 1; i < imagePaths.size(); i++)
		{
			imgData.push_back(SOIL_load_image(imagePaths[i].c_str(), &compWidth, &compHeight, NULL, 0));

			// of one of the images isnt the same size as the original then remove resources and exit function
			if (compWidth != width || compHeight != height || imgData.back() == nullptr)
			{
				//clear the loaded img data
				for (unsigned int j = 0; j < imgData.size(); j++)
				{
					SOIL_free_image_data(imgData[j]);
				}

				imgData.clear();

				return nullptr;
			}
		}

		int mipLevels = static_cast<int>(1 + floor(log2(std::max(width, height))));

		GLTexture* tex = new GLTexture;
		tex->type = GL_TEXTURE_2D_ARRAY;

		glGenTextures(1, &tex->id);
		glBindTexture(tex->type, tex->id);
		glTexStorage3D(tex->type, mipLevels, GL_RGB8, width, height, imgData.size());

		for (unsigned int i = 0; i < imgData.size(); i++)
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, imgData[i]);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (genMipMaps)
		{
			glTexParameterf(tex->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(tex->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(tex->type);
		}
		else
		{
			glTexParameteri(tex->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(tex->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}


		//TODO check if capable // anisostropic texture filtering
		float amount = 0.0f; 
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &amount);
		std::cout << "max: " << amount << std::endl;
		glTexParameterf(tex->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);

		for (unsigned int i = 0; i < imagePaths.size(); i++)
		{
			SOIL_free_image_data(imgData[i]);
		}

		imgData.clear();

		return tex;
	}
}