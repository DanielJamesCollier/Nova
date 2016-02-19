#pragma once

#define GLM_FORCE_RADIANS

#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>
#include "Window.h"

namespace Nova
{
	class Camera
	{
	public:
		Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar);
		~Camera();

		glm::mat4 GetViewProject() const;
		const glm::mat4& GetProjection() const;

		const glm::vec3& GetPosition() const;
		const glm::vec3& GetDirection() const;

		float GetAspectRatio() const;
		float GetFov() const;

		void Update(float delta);

	private:
		glm::mat4 m_perspective;
		glm::vec3 m_position;
		glm::vec3 m_forward;
		glm::vec3 m_up;
		glm::vec2 m_windowCentreXY;

		///*Movement Data*///
		float m_moveZSpeed = 0.0f;
		float m_moveXSpeed = 0.0f;
		float m_maxMoveSpeed  = 10.0f;
		float m_minMoveSpeed = -m_maxMoveSpeed;
		float m_moveAcceleration = 0.1f;
		bool  m_moving = false;
		/////////////////////

		///*Rotation Data*///
		glm::vec2 lastMouse     = glm::vec2();
		float m_rotXSpeed       = 0.0f;
		float m_rotYSpeed       = 0.0f;
		float m_rotMaxSpeed     = 10.0f;
		float m_rotMinSpeed     = -m_rotMaxSpeed;
		float m_rotAcceleration = 0.1f;
		/////////////////////

		//*FOV WARP*//
		float m_fovOrigional;
		float m_maxFov;
		float m_minFov;
		float m_fovIncrement = 4.0f;
		//////////////////

		struct
		{
			float m_fov;
			float m_aspect;
			float m_zNear;
			float m_zFar;
		} m_projectionInfo;
		
	
	};
}
