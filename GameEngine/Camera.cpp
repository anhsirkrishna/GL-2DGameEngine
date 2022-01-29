/******************************************************************************/
/*!
/*File: Camera.cpp
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   1/27/2022
/*  Implementation of the Camera Class
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#include "Camera.h"

// Updates the camera position based on the direction enum
void Camera::ProcessKeyboardInput(CameraMovement direction, float dt)
{
	float velocity = movement_speed * dt;

	switch (direction)
	{
	case CAM_FORWARD:
		position += front * velocity;
		break;
	case CAM_BACKWARD:
		position -= front * velocity;
		break;
	case CAM_LEFT:
		position -= right * velocity;
		break;
	case CAM_RIGHT:
		position += right * velocity;
		break;
	case CAM_UP:
		position += up * velocity;
		break;
	case CAM_DOWN:
		position -= up * velocity;
		break;
	default:
		break;
	}
}

// Updates the camera zoom based on a direction enum
void Camera::ProcessZoom(CameraMovement direction)
{
	if (direction == CAM_ZOOMIN)
		zoom -= ZOOM_SPEED;

	if (direction == CAM_ZOOMOUT)
		zoom += ZOOM_SPEED;

	if (zoom < 1.0f)
		zoom = 1.0f;

	if (zoom > 45.0f)
		zoom = 45.0f;
}


// Updates the camera vectors based on changes of yaw and pitch (mainly for mouse movement)
void Camera::UpdateCameraVectors()
{
	glm::vec3 temp_front;
	temp_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	temp_front.y = sin(glm::radians(pitch));
	temp_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, world_up));
	up = glm::normalize(glm::cross(right, front));
}