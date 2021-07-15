#pragma once
#include <glm/glm.hpp>

enum class KEY {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Camera
{
public:
	const float DEFAULT_YAW = -90.0f;
	const float DEFAULT_PITCH = 0.0f;
	const float DEFAULT_ROTATION_SPEED = 30.0f;
	const float DEFAULT_SENSITIVITY = 0.1f;
	const float DEFAULT_ZOOM = 45.0f; // FOV

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	glm::vec3 target;

	float cameraSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(glm::vec3 position, glm::vec3 worldUp, glm::vec3 target);

	void processMovement(KEY key, float deltaTime);
	void tiltCamera(float yOffset);
	void panCamera(float xOffset);
	void zoomCamera(float yOffset);

	glm::mat4 getViewMatrix();

private:
	void rotateCameraGlobal(float factor, glm::vec3 axis);
};