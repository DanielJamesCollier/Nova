#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <GLM/glm.hpp>

namespace Nova
{
	class Utils
	{
	public:
		static void split(const std::string &s, char delim, std::vector<std::string> &elems);

		static std::vector<std::string> split(const std::string &s, char delim);

		static glm::vec3 Convert_RGB_ToGLSLColorSpace(float r, float g, float b);
		static glm::vec3 Convert_RGB_ToGLSLColorSpace(glm::vec3 colour);

		static glm::vec4 Convert_RGBA_To_GLSL_Colour_Space(float r, float g, float b, float a);
		static glm::vec4 Convert_RGBA_To_GLSL_Colour_Space(glm::vec4 colour);
	};
}
#endif