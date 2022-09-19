#include<iostream>

#define	GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include"Shader.h"
#include"Camera.h"
#include"Material.h"
#include"LightDirectional.h"
#include"LightPoint.h"
#include"LightSpot.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma region Model Data

float vertices[] = {
	//position             //normal             //TexCoord 
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 1.0f,

};
glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
#pragma endregion

#pragma region Camera Declare
//ʵ�������  position �������λ��  pitch yaw  worldup �������ϵ�����
Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(15.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));//180�ȵ���Z�ᷴ��ŷ����Ҫע������ϵ
#pragma endregion

#pragma region Light Declare
// �۹�
LightSpot light = LightSpot(glm::vec3(0.0f, 5.0f, -3.0f), glm::vec3(glm::radians(90.0f),0.0f,0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f));

#pragma endregion

#pragma region Input Declare
float lastX;
float lastY;
bool fisrstMouse = true;


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//���������Escape
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//���������W
	{
		camera.speedZ = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)//���������S
	{
		camera.speedZ = -1.0f;
	}
	else
	{
		camera.speedZ = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.speedX = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.speedX = -1.0f;
	}
	else
	{
		camera.speedX = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.speedY = -1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.speedY = +1.0f;
	}
	else
	{
		camera.speedY = 0;
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	// if�����һ�ν�������
	if (fisrstMouse == true)
	{
		lastX = xPos;
		lastY = yPos;
		fisrstMouse = false;
	}
	float deltaX, deltaY;
	deltaX = xPos - lastX;
	deltaY = yPos - lastY;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(deltaX, deltaY);
}
#pragma endregion

unsigned int LoadImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot) {
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);
	glActiveTexture(GL_TEXTURE0 + textureSlot);//ָ����Texture��0ͨ��
	glBindTexture(GL_TEXTURE_2D, TexBuffer);

	int width, height, nrChannel;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("load image failed");
	}
	stbi_image_free(data);//�ͷŵ�����
	return TexBuffer;
}

int main() {
#pragma region Open a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//���汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile)

	//Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);

	if (window == NULL)
	{
		printf("Open window failed.");
		glfwTerminate();
		return -1;// -1 �������
	}
	glfwMakeContextCurrent(window);	/*�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳�����������
									  OpenGL��״̬ͨ������ΪOpenGL������(Context)��
									  ͨ������ʹ������;��ȥ����OpenGL״̬������ѡ��������塣�������ʹ�õ�ǰOpenGL����������Ⱦ��*/
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//�رչ��
	glfwSetCursorPosCallback(window, mouse_callback);

	//Init GLEW
	//��ʼ��GLEW����ʵ�����ʵĹ����Լ���������
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("Init GLEW failed.");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
#pragma endregion

#pragma region Init Shader Program
	Shader* myShader = new Shader("vertexSource.vert", "fragmentSource.frag");
#pragma endregion

#pragma region ��������Init Material
	Material* myMaterial = new Material(myShader,
		LoadImageToGPU("container2.png", GL_RGBA, GL_RGBA, Shader::DIFFUSE),//diffuse
		LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, Shader::SPECULAR),//specular
		LoadImageToGPU("matrix.jpg", GL_RGB, GL_RGB, Shader::EMISSION),
		glm::vec3(1.0f, 1.0f, 1.0f), //ambient
		32.0f);//shininess
#pragma endregion

#pragma region Init and Load Models to VAO, VBO
	unsigned int VAO;//VAO����
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;//VBO����
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//����������εĶ���Ž�VBO���棬����������GPU��buffer����ȥ
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// λ������
	//glVertexAttribPointer(������ţ�Ψ���������������ͣ��Ƿ��һ�����ֽ�ƫ��������һ��������ֽ�ƫ��������ʼλ�ã�)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//// ��ɫ����
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// ����
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// UV
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);

#pragma endregion

	stbi_set_flip_vertically_on_load(true);//���µߵ�

#pragma region Init and Load Textures
	/*unsigned int TexBufferA;
	TexBufferA = LoadImageToGPU("container.jpg", GL_RGB, GL_RGB, 0);
	unsigned int TexBufferB;
	TexBufferB = LoadImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1);*/
#pragma endregion

#pragma region ׼��MVP����
	// calculate our transformation matrix here
	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projMat;//P���󼸺������
	projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
#pragma endregion

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))//���ĳ�����Ƿ񱻹ر�
	{
		// ����
		processInput(window);
		// ����
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����һ֡����ɫ�������Ȼ���

		viewMat = camera.GetViewMatrix();

		for (int i = 0; i < 10; i++)
		{
			// ����M����
			modelMat = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			// ����V����

			// ����P����


			// ���ò��� -> Shader����
			myShader->use();
			// ���ò��� -> ��ͼ
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, myMaterial->diffuse);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, myMaterial->specular);
			// ���ò��� -> Uniforms
			/*glUniform1i(glGetUniformLocation(myShader->ID, "ourTexture"), 0);
			glUniform1i(glGetUniformLocation(myShader->ID, "ourFace"), 1);*/
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(myShader->ID, "ambientColor"), 0.1f, 0.1f, 0.1f);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightPos"), light.position.x, light.position.y, light.position.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightColor"), light.color.x, light.color.y, light.color.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightDirUniform"), light.direction.x, light.direction.y, light.direction.z);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhyInner"), light.cosPhyInner);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhyOutter"), light.cosPhyOutter);
			/*glUniform1f(glGetUniformLocation(myShader->ID, "lightP.constant"), light.constant);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightP.linear"), light.linear);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightP.quadratic"), light.quadratic);*/

			glUniform3f(glGetUniformLocation(myShader->ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);
			glUniform1f(glGetUniformLocation(myShader->ID, "time"), glfwGetTime());

			//δ��װǰ���ø�ʽglUniform3f(glGetUniformLocation(myShader->ID, "material.ambient"), myMaterial->ambient);
			myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
			myMaterial->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
			myMaterial->shader->SetUniform1i("material.specular", Shader::SPECULAR);
			myMaterial->shader->SetUniform1i("material.emission", Shader::EMISSION);
			myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);

			// ����ģ��
			glBindVertexArray(VAO);

			// Drawcall
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// ��鲢������-������������ ��Ϊ��һ����Ⱦѭ����׼��
		glfwSwapBuffers(window);//����window����ɫ������������һ���洢��GLFW����ÿһ��������ɫ�Ļ���
		glfwPollEvents();//���´���״̬�����ú���
		camera.UpdateCameraPos();
	}
	glfwTerminate();//��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
	return 0;
}