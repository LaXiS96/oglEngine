#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"

using namespace std;

Shader::Shader(GLenum type, const string& name) {
	/* Read source file */
	string sourceString;
	ifstream file;

	file.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		// TODO use ResourceManager
		file.open("resources/shaders/" + name);

		stringstream stream;
		stream << file.rdbuf();

		file.close();

		sourceString = stream.str();
	}
	catch (ifstream::failure e) {
		cout << "Shader: error reading shader files" << endl;
	}
	const char* sourceChar = sourceString.c_str();

	/* Compile source */
	GLint success;
	char infoLog[512];

	Id = glCreateShader(type);
	glShaderSource(Id, 1, &sourceChar, nullptr);
	glCompileShader(Id);
	glGetShaderiv(Id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(Id, 512, nullptr, infoLog);
		cout << "Shader: error compiling vertex shader: " << infoLog << endl;
	}
}
