#include "Utils.h"

namespace Nova
{
	void Utils::split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
	}

	std::vector<std::string> Utils::split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}


	glm::vec3 Utils::Convert_RGB_ToGLSLColorSpace(float r, float g, float b)
	{
		r /= 255;
		g /= 255;
		b /= 255;

		return glm::vec3(r,b,b);
	}

	glm::vec3 Utils::Convert_RGB_ToGLSLColorSpace(glm::vec3 colour)
	{
		colour.x /= 255;
		colour.y /= 255;
		colour.z /= 255;

		return colour;
	}

	glm::vec4 Utils::Convert_RGBA_To_GLSL_Colour_Space(float r, float g, float b, float a)
	{
		r /= 255;
		g /= 255;
		b /= 255;
		a /= 255;
		return glm::vec4(r,g,b,a);
	}

	glm::vec4 Utils::Convert_RGBA_To_GLSL_Colour_Space(glm::vec4 colour)
	{
		colour.x /= 255;
		colour.y /= 255;
		colour.z /= 255;
		colour.w /= 255;
		return colour;
	}
}
