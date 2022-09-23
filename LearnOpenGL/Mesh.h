#pragma once
#include <glm/glm.hpp>
#include<string>
#include<vector>
#include"Shader.h"
#include <GL/glew.h>
#include<iostream>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	/*  ��������  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	/*  ����  */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void DrawArray(Shader* shader, int diffuse, int specular, int emission);
	void DrawElements(Shader* shader);
	/*  ����  */
	Mesh(float vertices[]);

private:
	/* ��Ⱦ���� */
	unsigned int VAO, VBO, EBO;
	void setupElementsMesh();
	void setupArrayMesh();
};

