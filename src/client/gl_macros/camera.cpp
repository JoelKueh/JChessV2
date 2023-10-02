#include "camera.h"
// DEBUG
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)),
	movement_speed(SPEED),
	mouse_sensitivity(SENSITIVITY),
	zoom(ZOOM)
{
	this->position = position;
	this->up = up;
	this->yaw = yaw;
	this->pitch = pitch;
	update_camera_vectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z,
		float up_x, float up_y, float up_z,
		float yaw, float pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)),
	movement_speed(SPEED),
	mouse_sensitivity(SENSITIVITY),
	zoom(ZOOM)
{
	this->position = glm::vec3(pos_x, pos_y, pos_z);
	this->up = glm::vec3(up_x, up_y, up_z);
	this->yaw = yaw;
	this->pitch = pitch;
	update_camera_vectors();
}

glm::mat4 Camera::get_view_matr()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::move_cam_relative(float fvel, float rvel, float uvel,
		float speed, float deltaTime)
{
	glm::vec3 dir = glm::vec3(0, 0, 0);
	dir += fvel * front;
	dir += rvel * right;
	dir += uvel * up;
	glm::vec3 delta = dir * speed * movement_speed * deltaTime;
	position += delta;
}

void Camera::turn_cam(float yaw_delta, float pitch_delta)
{
	yaw += yaw_delta * mouse_sensitivity;
	pitch += pitch_delta * mouse_sensitivity;

	if (pitch > 89.9f) pitch = 89.9;
	else if (pitch < -89.9f) pitch = -89.9f;

	update_camera_vectors();
}

void Camera::update_zoom(float zoom_delta)
{
	zoom += zoom_delta;
	if (zoom < 1.0f) zoom = 1.0f;
	else if (zoom > 45.0f) zoom = 45.0f;
}

void Camera::update_camera_vectors()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, up));
	up = world_up;
}
