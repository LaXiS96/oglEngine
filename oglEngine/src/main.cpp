#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/version.h>

#include <iostream>
#include <iomanip>

#include "Shader.h"
#include "Model.h"
#include "Engine.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

using namespace std;

int main(int argc, char** argv) {

	cout << "oglEngine v0" << endl << endl;

	SDL_version SDLcompiled;
	SDL_version SDLlinked;
	SDL_VERSION(&SDLcompiled);
	SDL_GetVersion(&SDLlinked);
	cout << "libSDL2 compiled: " << (int)SDLcompiled.major << '.' << (int)SDLcompiled.minor << '.' << (int)SDLcompiled.patch
		<< " linked: " << (int)SDLlinked.major << '.' << (int)SDLlinked.minor << '.' << (int)SDLlinked.patch << endl;
	cout << "assimp " << aiGetVersionMajor() << '.' << aiGetVersionMinor() << '.' << aiGetVersionRevision() << endl;
	cout << "glm " << GLM_VERSION_MAJOR << '.' << GLM_VERSION_MINOR << '.' << GLM_VERSION_PATCH << '.' << GLM_VERSION_REVISION << endl;
	cout << "glad 0.1.33" << endl;
	cout << "stb_image 2.25" << endl;
	cout << endl;

	/*cout << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << endl;
	cout << "OpenGL Shading Language Version: " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	cout << "OpenGL Vendor:" << (char*)glGetString(GL_VENDOR) << endl;
	cout << "OpenGL Renderer:" << (char*)glGetString(GL_RENDERER) << endl;*/

	Engine engine;

	engine.Run();

	/*Uint32 fpsStartTicks = SDL_GetTicks();
	Uint32 fpsFramesCount = 0;

	while (running) {
		fpsFramesCount++;
		Uint32 fpsCurrentTicks = SDL_GetTicks();
		if (fpsCurrentTicks - fpsStartTicks >= 1000) {
			cout << "FPS: " << fpsFramesCount / ((fpsCurrentTicks - fpsStartTicks) / 1000.0f) << endl;
			fpsFramesCount = 0;
			fpsStartTicks = SDL_GetTicks();
		}
	}*/

	return 0;
}
