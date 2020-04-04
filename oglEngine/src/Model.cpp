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

void Model::loadModel(std::string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Model::loadModel: error importing model" << importer.GetErrorString() << std::endl;
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
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	// Read vertex positions, normals and texture coordinates
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vec;

		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		vertex.Position = vec;

		vec.x = mesh->mNormals[i].x;
		vec.y = mesh->mNormals[i].y;
		vec.z = mesh->mNormals[i].z;
		vertex.Normal = vec;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 coords;
			coords.x = mesh->mTextureCoords[0][i].x;
			coords.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = coords;
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

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = loadTextureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		textures.push_back(texture);
	}
	return textures;
}

GLuint Model::loadTextureFromFile(const char* path, const std::string directory) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	GLuint textureId;
	glGenTextures(1, &textureId);

	GLint width, height, componentsNr;
	stbi_uc* data = stbi_load(filename.c_str(), &width, &height, &componentsNr, 0);
	if (data) {
		GLenum format;
		if (componentsNr == 1)
			format = GL_RED;
		else if (componentsNr == 3)
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
		std::cout << "Model::loadTextureFromFile: error loading texture from " << filename << std::endl;
		stbi_image_free(data);
	}

	return textureId;
}
