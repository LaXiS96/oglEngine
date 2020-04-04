#include <glm\ext\matrix_transform.hpp>

#include "Camera.h"

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch) :
	Front(vec3(0.f, 0.f, -1.f)),
	MovementSpeed(3.f),
	MouseSensitivity(0.05f),
	MovementState(0)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;

	updateVectors();
}

mat4 Camera::GetViewMatrix() {
	return lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(float deltaTime) {
	float velocity = MovementSpeed * (deltaTime / 1000.f);
	if (MovementState & CameraMovement::Forward)
		Position += Front * velocity;
	if (MovementState & CameraMovement::Backward)
		Position -= Front * velocity;
	if (MovementState & CameraMovement::Left)
		Position -= Right * velocity;
	if (MovementState & CameraMovement::Right)
		Position += Right * velocity;
}

void Camera::ProcessMouse(float xOffset, float yOffset) {
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch -= yOffset;

	if (Pitch > 89.f)
		Pitch = 89.f;
	if (Pitch < -89.f)
		Pitch = -89.f;

	updateVectors();
}

void Camera::updateVectors() {
	vec3 front;
	front.x = cos(radians(Yaw)) * cos(radians(Pitch));
	front.y = sin(radians(Pitch));
	front.z = sin(radians(Yaw)) * cos(radians(Pitch));

	Front = normalize(front);
	Right = normalize(cross(Front, WorldUp));
	Up = normalize(cross(Right, Front));
}
