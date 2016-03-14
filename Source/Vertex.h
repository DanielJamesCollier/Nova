#pragma once

#include "GLM\glm.hpp"

struct Vertex
{
public:
	
	Vertex(const glm::vec3& p = glm::vec3(), const glm::vec2& t = glm::vec2(), const glm::vec3& n = glm::vec3(0,1,0), const glm::vec3& tg = glm::vec3(0,1,0))
	{
		pos  = p;
		tex  = t;
		norm = n;
		tang = tg;
	}

	glm::vec3 pos; // 0 offset
	glm::vec2 tex; // 28 offset
	glm::vec3 norm;
	glm::vec3 tang;

	// total size 36

};

