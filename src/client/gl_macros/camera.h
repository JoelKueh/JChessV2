#ifndef M_CAMERA_H
#define M_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 20.0f;
const float SENSITIVITY = 0.01f;
const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 world_up { 0.0f, 1.0f, 0.0f };
	
	float yaw;
	float pitch;

	float movement_speed;
	float mouse_sensitivity;
	float zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW, float pitch = PITCH);

	Camera(float pos_x, float pos_y, float pos_z,
			float up_x, float up_y, float up_z,
			float yaw, float pitch);

	glm::mat4 get_view_matr();
	void move_cam_relative(float fvel, float rvel, float uvel,
		float speed, float deltaTime);
	void turn_cam(float yaw_delta, float pitch_delta);
	void update_zoom(float zoom_delta);

private:
	void update_camera_vectors();
};

#endif
