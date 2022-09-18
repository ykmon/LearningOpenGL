#pragma once

int main() {
#pragma region Open a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//副版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//告诉GLFW我们使用的是核心模式(Core-profile)

																  //Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);

	if (window == NULL)
	{
		printf("Open window failed.");
		glfwTerminate();
		return -1;// -1 程序结束
	}
	glfwMakeContextCurrent(window);	/*将我们窗口的上下文设置为当前线程的主上下文了
									OpenGL的状态通常被称为OpenGL上下文(Context)。
									通常我们使用如下途径去更改OpenGL状态：设置选项，操作缓冲。最后，我们使用当前OpenGL上下文来渲染。*/
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//关闭光标
	glfwSetCursorPosCallback(window, mouse_callback);

	//Init GLEW
	//初始化GLEW，打开实验性质的功能以及错误提醒
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

#pragma region 材质属性Init Material
	Material* myMaterial = new Material(myShader,
		LoadImageToGPU("container2.png", GL_RGBA, GL_RGBA, Shader::DIFFUSE),//diffuse
		LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, Shader::SPECULAR),//specular
		LoadImageToGPU("matrix.jpg", GL_RGB, GL_RGB, Shader::EMISSION),
		glm::vec3(1.0f, 1.0f, 1.0f), //ambient
		32.0f);//shininess
#pragma endregion

#pragma region Init and Load Models to VAO, VBO
	unsigned int VAO;//VAO建立
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;//VBO建立
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//把这个三角形的顶点放进VBO里面，即让它进到GPU的buffer里面去
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 位置属性
	//glVertexAttribPointer(索引序号，唯独数量，数据类型，是否归一化，字节偏移量，第一个组件的字节偏移量（起始位置）)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//// 颜色属性
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// 法线
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// UV
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);

#pragma endregion

	stbi_set_flip_vertically_on_load(true);//上下颠倒

#pragma region Init and Load Textures
										   /*unsigned int TexBufferA;
										   TexBufferA = LoadImageToGPU("container.jpg", GL_RGB, GL_RGB, 0);
										   unsigned int TexBufferB;
										   TexBufferB = LoadImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1);*/
#pragma endregion

#pragma region 准备MVP矩阵
										   // calculate our transformation matrix here
	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projMat;//P矩阵几乎不会变
	projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
#pragma endregion

	// 渲染循环
	while (!glfwWindowShouldClose(window))//检测某个窗是否被关闭
	{
		// 输入
		processInput(window);
		// 清屏
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除上一帧的颜色缓冲和深度缓冲

		viewMat = camera.GetViewMatrix();

		for (int i = 0; i < 10; i++)
		{
			// 设置M矩阵
			modelMat = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			// 设置V矩阵

			// 设置P矩阵


			// 设置材质 -> Shader代码
			myShader->use();
			// 设置材质 -> 贴图
			/*glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TexBufferA);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, TexBufferB);*/
			// 设置材质 -> Uniforms
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

			//未封装前调用格式glUniform3f(glGetUniformLocation(myShader->ID, "material.ambient"), myMaterial->ambient);
			myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
			myMaterial->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
			myMaterial->shader->SetUniform1i("material.specular", Shader::SPECULAR);
			myMaterial->shader->SetUniform1i("material.emission", Shader::EMISSION);
			myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);

			// 设置模型
			glBindVertexArray(VAO);

			// Drawcall
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// 检查并调用事-件，交换缓冲 并为下一次渲染循环做准备
		glfwSwapBuffers(window);//交换window的颜色缓存区，它是一个存储着GLFW窗口每一个像素颜色的缓冲
		glfwPollEvents();//更新窗口状态并调用函数
		camera.UpdateCameraPos();
	}
	glfwTerminate();//正确释放/删除之前的分配的所有资源
	return 0;
}
