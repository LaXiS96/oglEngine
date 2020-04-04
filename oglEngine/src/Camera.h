#pragma once

#include <glm/glm.hpp>

using namespace glm;

enum CameraMovement {
	Forward = 1 << 0,
	Backward = 1 << 1,
	Left = 1 << 2,
	Right = 1 << 3
};

class Camera {
public:
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;

	int MovementState;

	Camera(
		vec3 position = vec3(0.f, 10.f, 10.f),
		vec3 up = vec3(0.f, 1.f, 0.f),
		float yaw = -90.f,
		float pitch = 0.f
	);

	mat4 GetViewMatrix();
	void ProcessKeyboard(float deltaTime);
	void ProcessMouse(float xOffset, float yOffset);

private:
	void updateVectors();
};
