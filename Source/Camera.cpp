#include "Camera.h"
#include "InputManager.h"
#include <SDL2\SDL.h>
#include <iostream>
#include "MathUtil.h"

using namespace std;

namespace Nova
{
	Camera::Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		m_projectionInfo.m_fov     = fov;
		m_projectionInfo.m_aspect  = aspect;
		m_projectionInfo.m_zNear   = zNear;
		m_projectionInfo.m_zFar    = zFar;

		m_perspective = glm::perspective(MathUtil::DegreesToRadiansf(m_projectionInfo.m_fov), m_projectionInfo.m_aspect, m_projectionInfo.m_zNear, m_projectionInfo.m_zFar);
		m_position    = pos;
		m_forward     = glm::vec3(0, 0, 1);
		m_up          = glm::vec3(0, 1, 0);

		m_fovOrigional = fov;
		m_maxFov       = fov + 5;
		m_minFov       = fov - 5;
	}

	Camera::~Camera()
	{
	}

	glm::mat4 Camera::GetViewProject() const
	{
		return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	const glm::mat4& Camera::GetProjection() const
	{
		return m_perspective;
	}

	const glm::vec3& Camera::GetPosition() const
	{
		return m_position;
	}

	const glm::vec3& Camera::GetDirection() const
	{
		return m_forward;
	}

	float Camera::GetAspectRatio() const
	{
		return m_projectionInfo.m_aspect;
	}

	float Camera::GetFov() const
	{
		return m_projectionInfo.m_fov;
	}

	void Camera::Update(float delta)
	{
		glm::vec3 right = glm::normalize(glm::cross(m_up, m_forward));
		glm::vec3 left = glm::normalize(glm::cross(m_forward, m_up));

		// positional movement
		{
			if (InputManager::IsKeyPressed(SDLK_w))
			{
				m_moving = true;

				if (m_moveZSpeed < m_maxMoveSpeed)
				{
					m_moveZSpeed += m_moveAcceleration;
				}
			}
			else if (InputManager::IsKeyPressed(SDLK_s))
			{
				m_moving = true;

				if (m_moveZSpeed > m_minMoveSpeed)
				{
					m_moveZSpeed -= m_moveAcceleration;
				}
			}

			if (InputManager::IsKeyPressed(SDLK_d))
			{
				m_moving = true;

				if (m_moveXSpeed < m_maxMoveSpeed)
				{
					m_moveXSpeed += m_moveAcceleration;
				}

			}
			else if (InputManager::IsKeyPressed(SDLK_a))
			{
				m_moving = true;

				if (m_moveXSpeed > m_minMoveSpeed)
				{
					m_moveXSpeed -= m_moveAcceleration;
				}
			}

			if (m_moving)
			{
				// apply fov scaling and rebuild the projection matrix
				/*if (m_projectionInfo.m_fov < m_maxFov)
					m_projectionInfo.m_fov += m_fovIncrement * delta;
				else
					m_projectionInfo.m_fov = m_maxFov;*/
			}
			else
			{
				if (m_moveZSpeed > 0)
				{
					m_moveZSpeed -= m_moveAcceleration;
					if (m_moveZSpeed < 0) m_moveZSpeed = 0;
				}
				else if (m_moveZSpeed < 0)
				{
					m_moveZSpeed += m_moveAcceleration;
					if (m_moveZSpeed > 0) m_moveZSpeed = 0;
				}


				if (m_moveXSpeed > 0)
				{
					m_moveXSpeed -= m_moveAcceleration;
					if (m_moveXSpeed < 0) m_moveXSpeed = 0;
				}
				else if (m_moveXSpeed < 0)
				{
					m_moveXSpeed += m_moveAcceleration;
					if (m_moveXSpeed > 0) m_moveXSpeed = 0;
				}


				//// return fov back to normal if not
				//if (m_projectionInfo.m_fov > m_fovOrigional)
				//{
				//	m_projectionInfo.m_fov -= m_fovIncrement * delta;
				//}
				//else
				//{
				//	m_projectionInfo.m_fov = m_fovOrigional;
				//}
				////cout << m_projectionInfo.m_fov << endl;
			}
			m_moving = false;

			m_position += (m_forward * (m_moveZSpeed * delta)) + (left * (m_moveXSpeed * delta));
			
			
		}

		// rotational movement
		{
			if (InputManager::WasMouseMoved())
			{
				lastMouse = InputManager::GetMouseDeltaXY();

		

				if (lastMouse.y > 0)
				{
					if (m_rotYSpeed < m_rotMaxSpeed)
					{
						m_rotYSpeed += m_rotAcceleration;
					}
					else
					{
						m_rotYSpeed = m_rotMaxSpeed;
					}
						
					
				}
				else if (lastMouse.y < 0)
				{
					if (m_rotYSpeed > m_rotMinSpeed)
					{
						m_rotYSpeed -= m_rotAcceleration;
					}
					else
					{
						m_rotYSpeed = m_rotMinSpeed;
					}
					
				}

				if (lastMouse.x < 0)
				{
					if (m_rotXSpeed < m_rotMaxSpeed)
					{
						m_rotXSpeed += m_rotAcceleration;
					}
					else
					{
						m_rotXSpeed = m_rotMaxSpeed;
					}
				}
				else if (lastMouse.x > 0)
				{
					if (m_rotXSpeed > m_rotMinSpeed)
					{
						m_rotXSpeed -= m_rotAcceleration;
					}
					else
					{
						m_rotXSpeed = m_rotMinSpeed;
					}
				}
			}
			else
			{
				if (m_rotYSpeed > 0)
				{
					m_rotYSpeed -= m_rotAcceleration;

					if (m_rotYSpeed < 0)
					{
						m_rotYSpeed = 0;
					}
				}
				else if (m_rotYSpeed < 0)
				{
					m_rotYSpeed += m_rotAcceleration;

					if (m_rotYSpeed > 0)
					{
						m_rotYSpeed = 0;
					}
				}

				if (m_rotXSpeed > 0)
				{
					m_rotXSpeed -= m_rotAcceleration;

					if (m_rotXSpeed < 0)
					{
						m_rotXSpeed = 0;
					}
				}
				else if (m_rotXSpeed < 0)
				{
					m_rotXSpeed += m_rotAcceleration;

					if (m_rotXSpeed > 0)
					{
						m_rotXSpeed = 0;
					}
				}
			}

			float rotX =  m_rotXSpeed * delta;
			float rotY =  m_rotYSpeed * delta;

			m_forward = glm::mat3(glm::rotate(rotX, m_up))  * m_forward;
			m_forward = glm::mat3(glm::rotate(rotY, right)) * m_forward;
			//m_forward *= m_rotSpeed * delta;
		}
	}
}

