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
#include "SkyBoxVertex.h"
#include "Texture.h"

using namespace std;

const unsigned width = 1000;
const unsigned height = 600;

//variables para el skybox
unsigned int VBOSKY, VAOSKY, EBOSKY;
unsigned int textureSky;

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
void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLight, Model ourModel, Shader ourShaderSky, Texture1 ourTextureSky);
//void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLight, Model ourModel);

void TransformCubo(Shader ourShader);
//void TransformCuboLight(Shader ourLight);
void TransformCamera(Shader ourShader, bool isSky);
void CameraUniform(Shader shaderName);
//metodos para usar el skybox
void GeneracionBufferSky();
void VertexAttribute(int layout, int data, int total, int start);

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
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("vertexShader_L.vs", "fragmentShader_L.fs");
	Model ourModel("Modelos/laberinto/untitled.obj");
	Shader ourLight("vertexLight.vl", "fragmentLight.fl");
	//generacion de shaders para el sky  
	Shader ourShaderSky("vertexShaderSky.vs", "fragmentShaderSky.fs");
	Texture1 ourTextureSky(textureSky);


	GeneracionBufferSky();
	ourTextureSky.GeneraTexturaSky(faces);
	ourShaderSky.use();
	ourShaderSky.setInt(ourTextureSky.UniformTextureSky(), 0);
	//setear el material
	ourShader.setInt("material.diffuse", 0);
	ourShader.setInt("material.specular", 1);


	updateWindow(window, ourShader, ourLight, ourModel, ourShaderSky, ourTextureSky);
	//updateWindow(window, ourShader, ourLight, ourModel);

	glDeleteVertexArrays(1, &VAOSKY);
	glDeleteBuffers(1, &VBOSKY);
	glDeleteBuffers(1, &EBOSKY);

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
void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLight, Model ourModel, Shader ourShaderSky, Texture1 ourTextureSky)
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
		//asinga el color del objeto
		ourShader.setVec3("material.ambient", 1.0f, 0.0f, 1.0f);
		ourShader.setVec3("material.diffuse", 1.0f, 0.0f, 1.0f);
		ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		ourShader.setFloat("material.shininess", 32.0f);


		//codigo para las luces
		//sigue siendo para el ourshader pero solo se le asignan los reflejos de luz
		vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 0.3f);
		lightColor.y = sin(glfwGetTime() * 0.5f);
		lightColor.z = sin(glfwGetTime() * 1.0f);

		vec3 diffuseColor = lightColor * vec3(0.3f);
		vec3 ambientColor = diffuseColor * vec3(0.5f);

		//para la luz con direcci�n
		ourShader.setVec3("light.direction", -0.9f, -1.0f, -0.3f);
		ourShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);



		TransformCamera(ourShader, false);
		TransformCubo(ourShader);
		//para dibujar el modelo 
		ourModel.Draw(ourShader);

		//sky
		glDepthFunc(GL_LEQUAL);
		ourShaderSky.use();
		TransformCamera(ourShaderSky, true);
		glBindVertexArray(VAOSKY);
		ourTextureSky.ViewTextureSky();
		glDepthFunc(GL_LESS);


		//la luz se sigue manejando aqui
		ourLight.use();
		CameraUniform(ourLight);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void TransformCubo(Shader ourShader)
{
	mat4 modelo = mat4(1.0f);
	modelo = translate(modelo, vec3(4.0f, 0.0f, 2.0f));
	modelo = scale(modelo, vec3(5.0f, 6.0f, 5.0f));
	ourShader.setMat4("model", modelo);
}

void TransformCamera(Shader ourShader, bool isSky )
{
	projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	if (isSky)
	{
		view = mat4(mat3(camera.GetViewMatrix()));
	}
	else
	{
		view = camera.GetViewMatrix();
	}

	CameraUniform(ourShader);
}

void CameraUniform(Shader shaderName)
{
	shaderName.setMat4("projection", projection);
	shaderName.setMat4("view", view);

}

void GeneracionBufferSky()
{
	glGenVertexArrays(1, &VAOSKY);
	glGenBuffers(1, &VBOSKY);
	glGenBuffers(1, &EBOSKY);

	glBindVertexArray(VAOSKY);

	glBindBuffer(GL_ARRAY_BUFFER, VBOSKY);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOSKY);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSky), indicesSky, GL_STATIC_DRAW);

	//como se agrego un layout mas se suman aqui
	VertexAttribute(0, 3, 8, 0);
	VertexAttribute(1, 3, 8, 3);
	VertexAttribute(2, 2, 8, 6);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void VertexAttribute(int layout, int data, int total, int start)
{
	glVertexAttribPointer(layout, data, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)(start * sizeof(float)));
	glEnableVertexAttribArray(layout);
}