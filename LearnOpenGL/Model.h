#pragma once
#include <vector>
#include <string>
#include "Mesh.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

class Model
{
//public:
//	Model(std::string path);
//	~Model();
//	std::vector<Mesh> meshes;
//	std::string directory;
//	void Draw(Shader* shader);
//
//private:
//	void loadModel(std::string path);
//	void processNode(aiNode* node, const aiScene* scene);
//	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

public:
	/* 函数 */
	Model(std::string path);
	void Draw(Shader* shader);

private:
	/* 模型数据 */
	std::vector<Mesh> meshes;
	std::string directory;
	/* 函数 */
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector <Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
	std::vector<Texture> textures_loaded;
};

