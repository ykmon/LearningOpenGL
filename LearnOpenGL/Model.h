#pragma once
#include<vector>
#include<string>
#include"Mesh.h"
#include"Shader.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<iostream>

class Model
{
public:
	Model(std::string path);
	~Model();
	std::vector<Mesh> meshes;
	std::string directory;
	void Draw(Shader* shader);

private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

