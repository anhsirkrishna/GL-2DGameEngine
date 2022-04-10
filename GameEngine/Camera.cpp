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
#include "FrameRateController.h"

// Updates the camera position based on the direction enum
void Camera::ProcessKeyboardInput(CameraMovement direction)
{
	float velocity = movement_speed * p_framerate_controller->GetPrevLoopDeltaTime();

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

glm::vec4 Camera::ScreenToWorld(int x, int y)
{
	float xnorm = (x / (float)window_width) * 2.0f - 1.0f;
	float ynorm = -(y / (float)window_height) * 2.0f + 1.0f;
	glm::vec4 screennorm(xnorm, -ynorm, -1.0f, 1.0f);

	glm::mat4 projview = GetProjMatrix() * GetViewMatrix();
	glm::mat4 projviewInv = glm::inverse(projview);

	glm::vec4 worldpos = projviewInv * screennorm;
	worldpos.w = 1.0f / worldpos.w;
	worldpos.x *= worldpos.w;
	worldpos.y *= worldpos.w;
	worldpos.z *= worldpos.w;

	return worldpos;
}

glm::mat4 Camera::GetViewMatrix() { return glm::lookAt(position, position + front, up); }
glm::mat4 Camera::GetProjMatrix() { return glm::perspective(glm::radians(zoom), (float)window_width / (float)window_height, 0.1f, 10000.0f); }

void Camera::ClampCameraPosition() {
	float lower_limit_x = 300;
	float lower_limit_y = 164;

	float upper_limit_x = 1496;
	float upper_limit_y = 730;

	float static_z = -500;

	//Clamp the x and y values between upper and lower limits
	//position.x = glm::min(glm::max(position.x, lower_limit_x), upper_limit_x);
	//position.y = glm::min(glm::max(position.y, lower_limit_y), upper_limit_y);
	position.z = static_z;
}

void Camera::SetWindowDimensions(int width, int height) {
	window_width = width;
	window_height = height;
}
