/*
 * Name:   Oliver Rankin-Starcevic
 * Number: 5800559
 * Email:  ors549@uowmail.edu.au
 * Title:  Assignment 1 Part 2
 * Date:   03/04/2019
 * Citations: Casey Chow - Tutorial3, Transformations, Draw Circle, GUI.
 */
#include <cstdio>		// for C++ i/o
#include <iostream>
#include <string>
#include <cstddef>
using namespace std;	// to avoid having to use std::

#include <GLEW/glew.h>	// include GLEW
#include <GLFW/glfw3.h>	// include GLFW (which includes the OpenGL header)
#include <glm/glm.hpp>	// include GLM (ideally should only use the GLM headers that are actually used)
#include <glm/gtx/transform.hpp>
using namespace glm;	// to avoid having to use glm::

#include <AntTweakBar.h>

#include "shader.h"

#define TOT_CUBES 5
#define PI 3.14159265
#define CIRCLE_RADIUS 1
#define MAX_VERTICES 40

// struct for vertex attributes
struct Vertex
{
	GLfloat position[3];
	GLfloat color[3];
};

// global variables

Vertex g_vertices[MAX_VERTICES] = {
	// vertex 1
	-0.5f, 0.5f, 0.5f,	// position
	1.0f, 0.0f, 1.0f,	// colour
	// vertex 2
	-0.5f, -0.5f, 0.5f,	// position
	1.0f, 0.0f, 0.0f,	// colour
	// vertex 3
	0.5f, 0.5f, 0.5f,	// position
	1.0f, 1.0f, 1.0f,	// colour
	// vertex 4
	0.5f, -0.5f, 0.5f,	// position
	1.0f, 1.0f, 0.0f,	// colour
	// vertex 5
	-0.5f, 0.5f, -0.5f,	// position
	0.0f, 0.0f, 1.0f,	// colour
	// vertex 6
	-0.5f, -0.5f, -0.5f,// position
	0.0f, 0.0f, 0.0f,	// colour
	// vertex 7
	0.5f, 0.5f, -0.5f,	// position
	0.0f, 1.0f, 1.0f,	// colour
	// vertex 8
	0.5f, -0.5f, -0.5f,	// position
	0.0f, 1.0f, 0.0f,	// colour
	// line loop
	0.0f, 0.0f, 0.0f,	//position
	1.0f, 0.0f, 0.0f,	// colour
};

GLuint g_indices[] = {
	0, 1, 2,	// triangle 1
	2, 1, 3,	// triangle 2
	4, 5, 0,	// triangle 3
	0, 5, 1,	// ...
	2, 3, 6,
	6, 3, 7,
	4, 0, 6,
	6, 0, 2,
	1, 5, 3,
	3, 5, 7,
	5, 4, 7,
	7, 4, 6,	// triangle 12
};

GLuint g_IBO = 0;				// index buffer object identifier
GLuint g_VBO = 0;				// vertex buffer object identifier
GLuint g_VAO = 0;				// vertex array object identifier
GLuint g_shaderProgramID = 0;	// shader program identifier
GLuint g_MVP_Index = 0;			// location in shader
glm::mat4 g_modelMatrix[TOT_CUBES * 2 - 1];		// object model matrices
glm::mat4 g_viewMatrix;			// view matrix
glm::mat4 g_projectionMatrix;	// projection matrix

float g_orbitSpeed[TOT_CUBES - 1] = { 0.05f, 0.1f, 0.2f, 0.3f };
float g_rotationSpeed[TOT_CUBES] = { 0.5f, 0.4f, 0.3f, 0.2f, 0.1f };
float g_size[TOT_CUBES] = { 0.8f, 0.5f, 0.2f, 0.33f, 2.0f };
float g_pos[TOT_CUBES - 1] = { 5.0f, 8.0f, 1.5f, 1.0f };

void generate_circle()
{
	float slice_angle = PI * 2 / static_cast<float>(32);	// angle for each fan slice
	float angle = slice_angle;								// angle used to generate x and y coordinates
	g_vertices[8].position[0] = CIRCLE_RADIUS;	// set x coordinate of vertex 8 (circle vertex 1)

	// generate vertex coordinates for line loop
	for (int i = 9; i < 40; i++) 
	{
		g_vertices[i].position[0] = CIRCLE_RADIUS * cos(angle); // x position
		g_vertices[i].position[1] = 0.0f; // y position
		g_vertices[i].position[2] = CIRCLE_RADIUS * sin(angle); // z position
		g_vertices[i].color[0] = 1.0f; // color (always red)
		g_vertices[i].color[1] = 0.0f;
		g_vertices[i].color[2] = 0.0f;

		angle += slice_angle; // update to next angle
	}
}

static void init(GLFWwindow* window)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// set clear background colour

	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// create and compile our GLSL program from the shader files
	g_shaderProgramID = loadShaders("VS.vert", "FS.frag");

	generate_circle();

	// find the location of shader variables
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint colorIndex = glGetAttribLocation(g_shaderProgramID, "aColor");
	g_MVP_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewProjectionMatrix");

	// initialise model matrix to the identity matrix
	for (int i = 0; i < TOT_CUBES * 2 - 1; i++)
	{
		g_modelMatrix[i] = glm::mat4(1.0f);
	}

	// initialise view matrix
	g_viewMatrix = glm::lookAt(glm::vec3(0.0f, 5.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Camera is at 0,5,5; Looking at origin; Head up;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	// initialise projection matrix
	g_projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f); 
	
	// generate identifier for VBO and copy data to GPU
	glGenBuffers(1, &g_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);

	// generate identifier for IBO and copy data to GPU
	glGenBuffers(1, &g_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);

	// generate identifiers for VAO
	glGenVertexArrays(1, &g_VAO);

	// create VAO and specify VBO data
	glBindVertexArray(g_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBO);
	// interleaved attributes
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

	glEnableVertexAttribArray(positionIndex);	// enable vertex attributes
	glEnableVertexAttribArray(colorIndex);
}

// function used to update the scene
static void update_scene()
{
	static float orbitAngle[TOT_CUBES - 1] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static float rotationAngle[TOT_CUBES] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	float scaleFactor = 0.1;

	// update rotation angles
	for (int i = 0; i < TOT_CUBES - 1; i++)
	{
		orbitAngle[i] += g_orbitSpeed[i] * scaleFactor;
		rotationAngle[i] += g_rotationSpeed[i] * scaleFactor;
	}
	rotationAngle[TOT_CUBES-1] += g_rotationSpeed[TOT_CUBES - 1] * scaleFactor;


	// update model matrix

	//sun
	g_modelMatrix[TOT_CUBES-1] = glm::rotate(rotationAngle[TOT_CUBES-1], glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::scale(glm::vec3(g_size[TOT_CUBES-1], g_size[TOT_CUBES-1], g_size[TOT_CUBES-1]));

	// planets
	for (int j = 0; j < 2; j++)
	{
		g_modelMatrix[j] = glm::rotate(orbitAngle[j], glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::translate(glm::vec3(g_pos[j], 0.0f, 0.0f))
			* glm::rotate(rotationAngle[j], glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::scale(glm::vec3(g_size[j], g_size[j], g_size[j]));

		// rings 1 & 2
		g_modelMatrix[TOT_CUBES + j] = glm::scale(glm::vec3(g_pos[j], 0.0f, g_pos[j]));
	}

	//moons
	for (int k = 2; k < 4; k++)
	{
		g_modelMatrix[k] = glm::rotate(orbitAngle[1], glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::translate(glm::vec3(g_pos[1], 0.0f, 0.0f)) // now in planet position
			* glm::rotate(orbitAngle[k], glm::vec3(0.0f, 1.0f, 0.0f)) 
			* glm::translate(glm::vec3(g_pos[k], 0.0f, 0.0f)) // move into orbit position
			* glm::rotate(rotationAngle[k], glm::vec3(0.0f, 1.0f, 0.0f)) // rotate on center
			* glm::scale(glm::vec3(g_size[k], g_size[k], g_size[k])); // set size

		// rings 1 & 2
		g_modelMatrix[TOT_CUBES + k] = glm::rotate(orbitAngle[1], glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::translate(glm::vec3(g_pos[1], 0.0f, 0.0f)) // now in planet position
			* glm::scale(glm::vec3(g_pos[k], 0.0f, g_pos[k]));
	}
}

// function used to render the scene
static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear colour buffer and depth buffer

	glUseProgram(g_shaderProgramID);	// use the shaders associated with the shader program

	glBindVertexArray(g_VAO);		// make VAO active

	glm::mat4 MVP;
	for (int i = 0; i < TOT_CUBES; i++)
	{
		MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[i];
		// set uniform model transformation matrix
		glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);	// display the vertices based on their indices and primitive type
	}
	for (int j = TOT_CUBES; j < TOT_CUBES * 2 - 1; j++)
	{
		MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[j];
		// set uniform model transformation matrix
		glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_LINE_LOOP, 8, 32);
	}
	
	glFlush();	// flush the pipeline
}

// key press or release callback function
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// quit if the ESCAPE key was press
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		// set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

// error callback function
static void error_callback(int error, const char* description)
{
	cerr << description << endl;	// output error description
}

int main(void)
{
	GLFWwindow* window = NULL;	// pointer to a GLFW window handle

	glfwSetErrorCallback(error_callback);	// set error callback function

	// initialise GLFW
	if (!glfwInit())
	{
		// if failed to initialise GLFW
		exit(EXIT_FAILURE);
	}

	// minimum OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window and its OpenGL context
	window = glfwCreateWindow(800, 600, "Assignment 1 - Part 2", NULL, NULL);

	// if failed to create window
	if (window == NULL)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);	// set window context as the current context
	glfwSwapInterval(1);			// swap buffer interval

	// initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		// if failed to initialise GLEW
		cerr << "GLEW initialisation failed" << endl;
		exit(EXIT_FAILURE);
	}

	// set key callback function
	glfwSetKeyCallback(window, key_callback);
	
	// initialise rendering states
	init(window);

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
			update_scene();		// update the scene
			render_scene();		// render the scene

			glfwSwapBuffers(window);	// swap buffers
			glfwPollEvents();			// poll for events
	}

	// clean up
	glDeleteProgram(g_shaderProgramID);
	glDeleteBuffers(1, &g_IBO);
	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);

	// uninitialise tweak bar
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

