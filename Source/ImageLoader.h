#pragma once
#ifndef IMAGE_LOADER_HEADER
#define IMAGE_LOADER_HEADER

#include <string>
#include <vector>

namespace Nova
{

	class GLTexture;

	class ImageLoader
	{
	public:

		/*Loader Functions*/
		/*--------------------------*/
		GLTexture* LoadTexture(const std::string& file_path, unsigned int texture_type, bool genMipmaps = true);
		static GLTexture* LoadImagesInto2DTexArray(const std::vector<std::string>& imagePaths, bool genMipmaps = true);
		GLTexture* LoadCubeMap(const std::string& posXFile, const std::string& negXFile, const std::string& posYFile, const std::string& negYFile, const std::string& posZFile, const std::string& negZFile);
		GLTexture* LoadImageNew(const std::string& filePath, unsigned int TextureType, bool genMips = true);
		/*--------------------------*/

	};
}



#endif //IMAGE_LOADER_HEADER