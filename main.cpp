#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <vector>

#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
#include "Model.h"

using namespace std;

const unsigned width = 1000;
const unsigned height = 600;

mat4 projection, view;

//variables para la camara
vec3 camPos = vec3(0.0f, 0.0f, 3.0f);
vec3 camFront = vec3(0.0f, 0.0f, -1.0f);
vec3 camUp = vec3(0.0f, 1.0f, 0.0f);

float deltime = 0.0f;
float lastTime = 0.0f;

//variables para la camara debuger
Camera camera(vec3(0.0f, 0.0f, 3.0f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

void initGLFWVersion();
bool gladLoad();

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);
void cameraInput(GLFWwindow* window);
//funciones para vista debug
void Mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLight, Model ourModel);
void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLight, Model ourModel);

void TransformCubo(Shader ourShader);
//void TransformCuboLight(Shader ourLight);
void TransformCamera(Shader ourShader);
void CameraUniform(Shader shaderName);

int main()
{
	initGLFWVersion();

	//Creacion de ventana
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGl", NULL, NULL);
	if (window == NULL)
	{
		cout << "Falla al cargar ventana" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoad())
	{
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//funciones para vista debug
	glfwSetCursorPosCallback(window, Mouse_callback);
	glfwSetScrollCallback(window, Scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//voltea la textura para evitar que no aparezca al reves
	//stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("vertexShaderC.vs", "fragmentShaderC.fs");
	Model ourModel("Modelos/laberinto/untitled.obj");
	Shader ourLight("vertexLight.vl", "fragmentLight.fl");

	updateWindow(window, ourShader, ourLight, ourModel);
	///updateWindow(window, ourShader, ourModel);

	glfwTerminate();
	return 0;
}

void initGLFWVersion()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
bool gladLoad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Falla al cargar glad" << endl;
		return false;
	}
	return true;
}
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	cameraInput(window);
}
void cameraInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS )
	{
		camera.ProcessKeyboard(FORWARD, deltime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(JUMP, deltime);
	}
}
void Mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = xposIn;
	//hace que la camara este en primera persona
	float ypos = 1;
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLight, Model ourModel)
{
	while (!glfwWindowShouldClose(window))
	{
		float currenTime = glfwGetTime();
		deltime = currenTime - lastTime;
		currenTime = lastTime;

		processInput(window);

	
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		TransformCamera(ourShader);
		TransformCubo(ourShader);
		//para dibujar el modelo 
		ourModel.Draw(ourShader);

		//la luz se sigue manejando aqui
		ourLight.use();
		CameraUniform(ourLight);
		//TransformCuboLight(ourLight); 

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void TransformCubo(Shader ourShader)
{
	mat4 modelo = mat4(1.0f);
	modelo = translate(modelo, vec3(4.0f, 3.0f, 2.0f));
	modelo = scale(modelo, vec3(5.0f, 6.0f, 5.0f));
	ourShader.setMat4("model", modelo);
}

/*
void TransformCuboLight(Shader ourLight)
{
	int tam = sizeof(posCubeLight) / sizeof(posCubeLight[0]);
	glBindVertexArray(VAO_L);
	for (int i = 0; i < tam; i++)
	{
		mat4 modelo = mat4(1.0f);
		modelo = translate(modelo, posCubeLight[i]);
		modelo = scale(modelo, vec3(0.2f));
		ourLight.setMat4("model", modelo);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}*/

void TransformCamera(Shader ourShader)
{
	projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	view = camera.GetViewMatrix();
	
	CameraUniform(ourShader);
}

void CameraUniform(Shader shaderName)
{
	shaderName.setMat4("projection", projection);
	shaderName.setMat4("view", view);

}