#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image.h>

#include <iostream>

#include "Model.h"

Model::Model(const char* path) {
	loadModel(path);
}

void Model::Draw(ShaderProgram shaderProgram) {
	for (GLuint i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shaderProgram);
}

void Model::loadModel(string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Model::loadModel: error importing model" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	// Read vertex positions, normals and texture coordinates
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		vertex.Position = vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		vertex.Normal = vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		vertex.Tangent = vec3(
			mesh->mTangents[i].x,
			mesh->mTangents[i].y,
			mesh->mTangents[i].z
		);

		vertex.Bitangent = vec3(
			mesh->mBitangents[i].x,
			mesh->mBitangents[i].y,
			mesh->mBitangents[i].z
		);

		if (mesh->mTextureCoords[0]) {
			glm::vec2 coords;
			coords.x = mesh->mTextureCoords[0][i].x;
			coords.y = mesh->mTextureCoords[0][i].y;
			vertex.TextureCoords = coords;
		}

		vertices.push_back(vertex);
	}

	// Read faces indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Read material textures
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
	vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = loadTextureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		cout << texture.id << " " << texture.type << " " << str.C_Str() << endl;
		textures.push_back(texture);
	}
	return textures;
}

GLuint Model::loadTextureFromFile(const char* path, const string directory) {
	string filename = string(path);
	filename = directory + '/' + filename;

	GLuint textureId;
	glGenTextures(1, &textureId);

	GLint width, height, componentsNr;
	stbi_uc* data = stbi_load(filename.c_str(), &width, &height, &componentsNr, 0);
	if (data) {
		GLenum format = GL_RED;
		if (componentsNr == 3)
			format = GL_RGB;
		else if (componentsNr == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		cout << "Model::loadTextureFromFile: error loading texture from " << filename << endl;
		stbi_image_free(data);
	}

	return textureId;
}
