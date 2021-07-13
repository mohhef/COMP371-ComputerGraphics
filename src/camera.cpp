#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include <iostream>
using namespace std;

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, glm::vec3 target)
{
	this->position = position;
	this->worldUp = worldUp;
	this->target = target;

	yaw = DEFAULT_YAW;
	pitch = DEFAULT_PITCH;

	cameraSpeed = DEFAULT_SPEED;
	mouseSensitivity = DEFAULT_SENSITIVITY;
	zoom = DEFAULT_ZOOM;

	updateCameraVectors();
}

void Camera::processMovement(KEY key, float deltaTime)
{ 
	// TODO Figure out factor/cameraSpeed optimal
	// float factor = cameraSpeed * deltaTime;
	float factor = 0.05f;

	if (key == KEY::UP)
		// position += glm::normalize(glm::cross(front, right)) * cameraSpeed;
		rotateGlobalX(factor);
	if (key == KEY::DOWN)
		// position -= glm::normalize(glm::cross(front, right)) * cameraSpeed;
		rotateGlobalX(-1 * factor);
	if (key == KEY::LEFT)
		// position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
		rotateGlobalY(factor);
	if (key == KEY::RIGHT)
		// position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
		rotateGlobalY(-1 * factor);
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, target, up);
}

// SHOULD USE THIS FOR PAN AND TILT ONLY AFTER INITIAL SETUP
void Camera::updateCameraVectors()
{
	glm::vec3 cameraFront;
	cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront.y = sin(glm::radians(pitch));
	cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(cameraFront);

	right = glm::normalize(glm::cross(front, worldUp));
	
	up = glm::normalize(glm::cross(right, front));
}

void Camera::rotateGlobalX(float factor)
{
	float y_value = position.y;
	float z_value = position.z;

	if ((sin(factor) * y_value + cos(factor) * z_value) < 0)
		return;

	position.y = cos(factor) * y_value - sin(factor) * z_value;
	position.z = sin(factor) * y_value + cos(factor) * z_value;

	y_value = front.y;
	z_value = front.z;
	front.y = cos(factor) * y_value - sin(factor) * z_value;
	front.z = sin(factor) * y_value + cos(factor) * z_value;

	y_value = up.y;
	z_value = up.z;
	up.y = cos(factor) * y_value - sin(factor) * z_value;
	up.z = sin(factor) * y_value + cos(factor) * z_value;
}

void Camera::rotateGlobalY(float factor)
{
	float x_value = position.x;
	float z_value = position.z;

	if ((-1 * sin(factor) * x_value + cos(factor) * z_value) < 0)
		return;

	position.x = cos(factor) * x_value + sin(factor) * z_value;
	position.z = -1 * sin(factor) * x_value + cos(factor) * z_value;

	x_value = front.x;
	z_value = front.z;
	front.x = cos(factor) * x_value + sin(factor) * z_value;
	front.z = -1 * sin(factor) * x_value + cos(factor) * z_value;

	x_value = right.x;
	z_value = right.z;
	right.x = cos(factor) * x_value + sin(factor) * z_value;
	right.z = -1 * sin(factor) * x_value + cos(factor) * z_value;
}