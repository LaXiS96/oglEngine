#pragma once

#include <glad/glad.h>

#include <string>

using namespace std;

class Shader {
public:
	GLuint Id;

	Shader(GLenum type, const string& name);
};
