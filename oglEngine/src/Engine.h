#pragma once

#include <SDL2/SDL.h>

#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"

struct FrameTimes {
	float lastFrame;
	float currentFrame;
	float deltaTime;
};

class Engine {
public:
	void Run();

private:
	bool running;
	unsigned int windowWidth = 1920;
	unsigned int windowHeight = 1080;

	SDL_Window* window;
	SDL_GLContext glContext;

	FrameTimes frameTimes;

	ShaderProgram* mainShaderProgram;
	ShaderProgram* lightShaderProgram;
	Camera* mainCamera;
	Model* mainModel;

	/* Lifecycle methods */
	void Init();
	void Loop();
	void Cleanup();

	void ProcessInput();
};
