#pragma once
#include <string>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);//���캯��
	std::string vertexString;
	std::string fragmentString;
	const char* vertexSource;//const���η��ú��������ֻ��д��һ��
	const char* fragmentSource;
	unsigned int ID;
	void use();

private:
	void checkCompileErrors(unsigned int ID, std::string type);
};
