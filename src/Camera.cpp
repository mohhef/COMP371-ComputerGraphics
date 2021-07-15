#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include <iostream>
using namespace std;

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, glm::vec3 target)
{
	this->position = position;
	this->worldUp = worldUp;
	this->target = target;

	cameraSpeed = DEFAULT_ROTATION_SPEED;
	mouseSensitivity = DEFAULT_SENSITIVITY;
	zoom = DEFAULT_ZOOM;

	glm::vec3 cameraFront;
	cameraFront.x = cos(glm::radians(DEFAULT_YAW)) * cos(glm::radians(DEFAULT_PITCH));
	cameraFront.y = sin(glm::radians(DEFAULT_PITCH));
	cameraFront.z = sin(glm::radians(DEFAULT_YAW)) * cos(glm::radians(DEFAULT_PITCH));
	front = glm::normalize(cameraFront);

	right = glm::normalize(glm::cross(front, worldUp));

	up = glm::normalize(glm::cross(right, front));
}

void Camera::processMovement(KEY key, float deltaTime)
{ 
	float factor = cameraSpeed * deltaTime;
	
	if (key == KEY::UP)
		rotateCameraGlobal(-1 * factor, glm::vec3(1.0f, 0.0f, 0.0f));
	if (key == KEY::DOWN)
		rotateCameraGlobal(factor, glm::vec3(1.0f, 0.0f, 0.0f));
	if (key == KEY::LEFT)
		rotateCameraGlobal(-1 * factor, glm::vec3(0.0f, 1.0f, 0.0f));
	if (key == KEY::RIGHT)
		rotateCameraGlobal(factor, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::tiltCamera(float yOffset)
{
	yOffset *= mouseSensitivity;

	front = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(yOffset), glm::vec3(1.0f, 0.0f, 0.0f))) * front;
	up = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(yOffset), glm::vec3(1.0f, 0.0f, 0.0f))) * up;
	right = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(yOffset), glm::vec3(1.0f, 0.0f, 0.0f))) * right;
}

void Camera::panCamera(float xOffset)
{
	xOffset *= mouseSensitivity;

	front = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(xOffset), glm::vec3(0.0f, 1.0f, 0.0f))) * front;
	up = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(xOffset), glm::vec3(0.0f, 1.0f, 0.0f))) * up;
	right = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(xOffset), glm::vec3(0.0f, 1.0f, 0.0f))) * right;
}

void Camera::zoomCamera(float yOffset)
{
	yOffset *= mouseSensitivity;
	zoom += yOffset;

	if (zoom < 10.0f)
		zoom = 10.0f;
	if (zoom > 75.0f)
		zoom = 75.0f;
}

void Camera::rotateCameraGlobal(float factor, glm::vec3 axis)
{	
	position = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(factor), axis)) * position;
	front = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(factor), axis)) * front;
	up = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(factor), axis)) * up;
	right = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(factor), axis)) * right;
}