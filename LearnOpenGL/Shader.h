#pragma once
#include <string>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);//构造函数
	std::string vertexString;
	std::string fragmentString;
	const char* vertexSource;//const修饰符让后面的内容只能写入一次
	const char* fragmentSource;
	unsigned int ID;
	void use();

private:
	void checkCompileErrors(unsigned int ID, std::string type);
};
