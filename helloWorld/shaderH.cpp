// 引入GLEW库 定义静态链接
#define GLEW_STATIC
#include <GL/glew.h>
// 引入GLFW库
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderHandle.h"
#include <cstdio>
// 键盘回调函数原型声明
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// 定义程序常量
static const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

/*
 * 绘制流程
 * 申请OVA 绑定OVA顶点数组对象
 * 申请OVB 绑定OVB顶点缓冲对象          顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
 * 填充OVB数据buffdata
 * 链接顶点属性、解释输入对应顶点数据
*/



static GLfloat vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
};

static GLuint indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

static GLuint createMyVertextVAO()
{
	GLuint resultOVA, OVB;
	glGenVertexArrays(1, &resultOVA);
	glGenBuffers(1, &OVB);
	glBindVertexArray(resultOVA);
	glBindBuffer(GL_ARRAY_BUFFER, OVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 填充数据
	//解释数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return resultOVA;
}

static GLuint createMyElementVAO()
{
	GLuint resultOVA, EBO, VBO;
	glGenVertexArrays(1, &resultOVA);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(resultOVA);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return resultOVA;
}

static const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
static const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";



static GLuint createShaderProgram(int shaderType, const GLchar** shaderStr)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, shaderStr, NULL);
	glCompileShader(shader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

int main(int argc, char** argv)
{

	if (!glfwInit())    // 初始化glfw库
	{
		std::cout << "Error::GLFW could not initialize GLFW!" << std::endl;
		return -1;
	}

	// 开启OpenGL 3.3 core profile
	std::cout << "Start OpenGL core profile version 3.3" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
		"Demo of createContext", NULL, NULL);
	if (!window)
	{
		std::cout << "Error::GLFW could not create winddow!" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 创建的窗口的context指定为当前context
	glfwMakeContextCurrent(window);

	// 注册窗口键盘事件回调函数
	glfwSetKeyCallback(window, key_callback);

	// 初始化GLEW 获取OpenGL函数
	glewExperimental = GL_TRUE; // 让glew获取所有拓展函数
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Error::GLEW glew version:" << glewGetString(GLEW_VERSION)
			<< " error string:" << glewGetErrorString(status) << std::endl;
		glfwTerminate();
		return -1;
	}

	// 设置视口参数
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	auto vertexVAO = createMyElementVAO();
	auto shaderProgram = ShaderHandle("vertexShader.vs", "framentShader_time_color.fs");
	//

	// 开始游戏主循环
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // 处理例如鼠标 键盘等事件

		// 清除颜色缓冲区 重置为指定颜色
		glClearColor(0.9f, 0.04f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLdouble curTime = glfwGetTime();
		GLdouble greenValue = (sin(curTime) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram.getProgram(), "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		std::printf("%.1f", greenValue);
		glBindVertexArray(vertexVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		shaderProgram.use();
		glfwSwapBuffers(window); // 交换缓存
	}
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glfwTerminate(); // 释放资源
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // 关闭窗口
	}
}