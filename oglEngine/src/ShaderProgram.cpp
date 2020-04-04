#include <fstream>
#include <sstream>
#include <iostream>

#include "ShaderProgram.h"

using namespace std;

ShaderProgram::ShaderProgram() {
	Id = glCreateProgram();
}

void ShaderProgram::Attach(Shader shader) {
	glAttachShader(Id, shader.Id);
}

void ShaderProgram::Link() {
	GLint success;
	char infoLog[512];

	glLinkProgram(Id);
	glGetProgramiv(Id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Id, 512, nullptr, infoLog);
		cout << "Shader: error linking shader program: " << infoLog << endl;
	}
}

void ShaderProgram::Use() {
	glUseProgram(Id);
}

void ShaderProgram::SetBool(const string& name, bool value) {
	glUniform1i(glGetUniformLocation(Id, name.c_str()), (GLint)value);
}

void ShaderProgram::SetInt(const string& name, int value) {
	glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
}

void ShaderProgram::SetFloat(const string& name, float value) {
	glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
}

void ShaderProgram::SetMat4(const string& name, float* value) {
	glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, value);
}
