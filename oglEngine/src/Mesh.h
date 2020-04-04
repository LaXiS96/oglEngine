#pragma once

#include <glad\glad.h>

#include <vector>

#include "Vertex.h"
#include "Texture.h"
#include "ShaderProgram.h"

class Mesh {
public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	void Draw(ShaderProgram shaderProgram);

private:
	GLuint VAO, VBO, EBO;

	void setupMesh();
};
