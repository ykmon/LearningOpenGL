#include "Model.h"
//Model::Model(std::string path)
//{
//	loadModel(path);
//}
//
//Model::~Model()
//{
//}
//
//void Model::Draw(Shader* shader)
//{
//	for (unsigned int i = 0; i < meshes.size(); i++)
//	{
//		meshes[i].Draw(shader);
//	}
//}
//
//void Model::loadModel(std::string path)
//{
//	Assimp::Importer import;
//	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)//���scene������ͱ���
//	{
//		std::cout << "Assimp error" << import.GetErrorString() << std::endl;
//		return;
//	}
//	directory = path.substr(0, path.find_last_of('\\')); //�������ַ�����Ҫ���ָ��λ�ÿ�ʼ��������ָ���ĳ��ȼ�ε�����
//	//std::cout << "Success!" << directory << std::endl;
//	processNode(scene->mRootNode, scene);
//}
//
//void Model::processNode(aiNode* node, const aiScene* scene)
//{
//	//std::cout << node->mName.data << std::endl;
//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh* curMesh = scene->mMeshes[node->mMeshes[i]];
//		meshes.push_back(processMesh(curMesh,scene));
//	}
//	for (unsigned int i = 0; i < node->mNumChildren; i++)
//	{
//		processNode(node->mChildren[i], scene);
//	}
//}
//
//Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
//{
//	std::vector<Vertex> tempVertices;
//	std::vector<unsigned int> tempIndices;
//	std::vector<Texture> tempTextures;
//
//	// ����Vertex
//	glm::vec3 tempVec;
//	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//	{
//		Vertex tempVertex;
//		// position
//		tempVertex.Position.x = mesh->mVertices[i].x;
//		tempVertex.Position.y = mesh->mVertices[i].y;
//		tempVertex.Position.z = mesh->mVertices[i].z;
//
//		// normal
//		tempVertex.Normal.x = mesh->mNormals[i].x;
//		tempVertex.Normal.y = mesh->mNormals[i].y;
//		tempVertex.Normal.z = mesh->mNormals[i].z;
//
//		// uv
//		if (mesh->mTextureCoords[0])//������ڵ�0��UV
//		{
//			tempVertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
//			tempVertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
//		}
//		else
//		{
//			tempVertex.TexCoords = glm::vec2(0, 0);
//		}
//		tempVertices.push_back(tempVertex);
//	}
//	// ����Indices
//	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//	{
//		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
//		{
//			tempIndices.push_back(mesh->mFaces[i].mIndices[j]);
//		}
//	}
//
//	return Mesh(tempVertices, tempIndices, {});
//}

Model::Model(std::string path)
{
	loadModel(path);
}


void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('\\'));
	std::cout << "Success!" << directory << std::endl;
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// ����ڵ����е�����
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// �������������ӽڵ��ظ���һ����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// ������λ�á����ߺ���������
		Vertex vertex;
		glm::vec3 vector;
		// λ��
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// ����
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// ��������
		if (mesh->mTextureCoords[0])// �Ƿ����
		{
			glm::vec2 vec;
			 // Assimp����һ��ģ����һ�������������8����ͬ���������꣬���ǲ����õ���ô�࣬����ֻ���ĵ�һ��
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// ��������
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// �������
	if (mesh->mMaterialIndex >= 0)//����в���
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMapls = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMapls.begin(), specularMapls.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)//�鴢���ڲ��������������
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			//int strcmp(const char *str1, const char *str2) �� str1 ��ָ����ַ����� str2 ��ָ����ַ������бȽϡ�
			if (std::strcmp(textures_loaded[j].path.data(),str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{	// �������û�б����أ��������
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);

		}
		
		Texture texture;
		texture.id = TextureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
		textures_loaded.push_back(texture); // ��ӵ��Ѽ��ص�������
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '\\' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Model::Draw(Shader* shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].DrawElements(shader);
	}
}