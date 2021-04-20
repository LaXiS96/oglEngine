#include <glad/glad.h>
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_clip_space.hpp>

#include <iostream>

#include "Engine.h"
#include "Shader.h"

using namespace std;

void Engine::Run() {
	Init();

	running = true;
	Loop();

	Cleanup();
}

void Engine::Init() {
	// TODO create logger and error checking functions
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL_Init error: " << SDL_GetError() << endl;
		//return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow(
		"oglEngine",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		cout << "SDL_CreateWindow error: " << SDL_GetError() << endl;
		//SDL_Quit();
		//return 1;
	}

	glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		cout << "SDL_GL_CreateContext error: " << SDL_GetError() << endl;
		//SDL_Quit();
		//return 1;
	}

	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		cout << "gladLoadGLLoader error" << endl;
		//SDL_Quit();
		//return 1;
	}

	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Shader mainVertexShader = Shader(GL_VERTEX_SHADER, "main.v.glsl");
	Shader mainFragmentShader = Shader(GL_FRAGMENT_SHADER, "main.f.glsl");
	mainShaderProgram = new ShaderProgram();
	mainShaderProgram->Attach(mainVertexShader);
	mainShaderProgram->Attach(mainFragmentShader);
	mainShaderProgram->Link();

	/*Shader lightVertexShader = Shader(GL_VERTEX_SHADER, "light.v.glsl");
	Shader lightFragmentShader = Shader(GL_FRAGMENT_SHADER, "light.f.glsl");
	lightShaderProgram = new ShaderProgram();
	lightShaderProgram->Attach(lightVertexShader);
	lightShaderProgram->Attach(lightFragmentShader);
	lightShaderProgram->Link();*/

	mainCamera = new Camera();

	mainModel = new Model("resources/models/nanosuit/nanosuit.obj");
}

void Engine::Loop() {
	mat4 model = mat4(1.f);
	mat4 view = mat4(1.f);
	mat4 projection = perspective(radians(45.f), (float)windowWidth / (float)windowHeight, 0.1f, 100.f);;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (running) {
		frameTimes.currentFrame = (float)SDL_GetTicks();
		frameTimes.deltaTime = frameTimes.currentFrame - frameTimes.lastFrame;
		frameTimes.lastFrame = frameTimes.currentFrame;

		ProcessInput();

		//model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
		view = mainCamera->GetViewMatrix();

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainShaderProgram->Use();

		mainShaderProgram->SetMat4("modelMatrix", model);
		mainShaderProgram->SetMat4("viewMatrix", view);
		mainShaderProgram->SetMat4("projectionMatrix", projection);

		mainShaderProgram->SetVec3("viewPosition", mainCamera->Position);
		vec3 lightDir = vec3(
			cos(radians((float)SDL_GetTicks() / 10.f)),
			sin(radians((float)SDL_GetTicks() / 10.f)),
			-1.f
		);
		mainShaderProgram->SetVec3("dirLight.direction", lightDir);
		mainShaderProgram->SetVec3("dirLight.ambient", vec3(0.5f, 0.5f, 0.5f));
		mainShaderProgram->SetVec3("dirLight.diffuse", vec3(1.f, 1.f, 1.f));
		mainShaderProgram->SetVec3("dirLight.specular", vec3(1.f, 1.f, 1.f));

		mainModel->Draw(*mainShaderProgram);

		SDL_GL_SwapWindow(window);
	}
}

void Engine::Cleanup() {
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Engine::ProcessInput() {
	SDL_Event event;

	float mouseX = 0.f;
	float mouseY = 0.f;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		}
		else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
			switch (event.key.keysym.sym) {
			case SDLK_w:
				mainCamera->MovementState |= CameraMovement::Forward;
				break;
			case SDLK_s:
				mainCamera->MovementState |= CameraMovement::Backward;
				break;
			case SDLK_a:
				mainCamera->MovementState |= CameraMovement::Left;
				break;
			case SDLK_d:
				mainCamera->MovementState |= CameraMovement::Right;
				break;
			case SDLK_ESCAPE:
				running = false;
				break;
			}
		}
		else if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
			case SDLK_w:
				mainCamera->MovementState &= ~CameraMovement::Forward;
				break;
			case SDLK_s:
				mainCamera->MovementState &= ~CameraMovement::Backward;
				break;
			case SDLK_a:
				mainCamera->MovementState &= ~CameraMovement::Left;
				break;
			case SDLK_d:
				mainCamera->MovementState &= ~CameraMovement::Right;
				break;
			}
		}
		else if (event.type == SDL_MOUSEMOTION) {
			mouseX = (float)event.motion.xrel;
			mouseY = (float)event.motion.yrel;
		}
	}

	mainCamera->ProcessKeyboard(frameTimes.deltaTime);
	mainCamera->ProcessMouse(mouseX, mouseY);
}
