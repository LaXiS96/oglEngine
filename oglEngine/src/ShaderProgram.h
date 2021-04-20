#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

#include "Shader.h"

using namespace std;
using namespace glm;

class ShaderProgram {
public:
	GLuint Id;

	ShaderProgram();

	void Attach(Shader shader);
	void Link();
	void Use();

	void SetBool(const string& name, bool value);
	void SetInt(const string& name, int value);
	void SetFloat(const string& name, float value);
	void SetVec3(const string& name, vec3 value);
	void SetMat4(const string& name, mat4 value);
};
