#pragma once

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
			/*glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TexBufferA);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, TexBufferB);*/
			// ���ò��� -> Uniforms
			/*glUniform1i(glGetUniformLocation(myShader->ID, "ourTexture"), 0);
			glUniform1i(glGetUniformLocation(myShader->ID, "ourFace"), 1);*/
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(myShader->ID, "ambientColor"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightPos"), light.position.x, light.position.y, light.position.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightColor"), light.color.x, light.color.y, light.color.z);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhy"), light.cosPhy);
			//glUniform3f(glGetUniformLocation(myShader->ID, "lightDirUniform"), light.direction.x, light.direction.y, light.direction.z);
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
