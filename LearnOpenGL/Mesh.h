#pragma once
#include<string>
#include <glm/glm.hpp>
#include<vector>
#include"Shader.h"
#include <GL/glew.h>

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
	Mesh(float vertices[]);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	void Draw(Shader* shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

