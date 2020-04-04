#pragma once

#include <assimp/scene.h>

#include <vector>

#include "ShaderProgram.h"
#include "Mesh.h"

class Model {
public:
	Model(const char* path);

	void Draw(ShaderProgram shaderProgram);

private:
	vector<Mesh> meshes;
	string directory;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	static GLuint loadTextureFromFile(const char* path, const string directory);
};
