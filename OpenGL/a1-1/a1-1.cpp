/*
 * Name:   Oliver Rankin-Starcevic
 * Number: 5800559
 * Email:  ors549@uowmail.edu.au
 * Title:  Assignment 1 Part 1
 * Date:   03/04/2019
 * Citations: Casey Chow - Tutorial3, Transformations, Draw Circle.
 */
#include <cstdio>		// for C++ i/o
#include <iostream>
#include <cstddef>
using namespace std;	// to avoid having to use std::

#include <GLEW/glew.h>	// include GLEW
#include <GLFW/glfw3.h>	// include GLFW (which includes the OpenGL header)
#include <glm/glm.hpp>	// include GLM (ideally should only use the GLM headers that are actually used)
#include <glm/gtx/transform.hpp>
using namespace glm;	// to avoid having to use glm::

#include "shader.h"

#define PI 3.14159265
#define CIRCLE_RADIUS 0.66
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define MAX_VERTICES 51
#define MODEL_MATRICES 14
#define ANIMATION_SPEED 0.005f

// struct for vertex attributes
struct Vertex
{
	GLfloat position[3];
	GLfloat color[3];
};

// global variables
Vertex g_vertices[MAX_VERTICES] = {
		// base
	// line
// vertex 1
	-1.0f, -0.8f, 0.0f,	// position
	0.5f, 0.5f, 0.5f,	// colour
// vertex 2
	1.0f, -0.8f, 0.0f,	// position
	0.5f, 0.5f, 0.5f,	// colour

	// triangle 1
// vertex 1
	-0.2f, -0.8f, 0.0f,	// position
	1.0f, 1.0f, 0.0f,	// colour
// vertex 2
	0.2f, -0.8f, 0.0f,	// position
	0.2f, 0.2f, 0.0f,	// colour
// vertex 3
	0.0f, 0.0f, 0.0f,	// position
	1.0f, 1.0f, 0.0f,	// colour

		// carriage
	//triangle 1
// vertex 1
	-0.05f, 0.05f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour
// vertex 2
	-0.075f, 0.0f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour
// vertex 3
	0.05f, 0.05f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour

	// triangle 2
// vertex 1
	0.05f, 0.05f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour
// vertex 2
	-0.075f, 0.0f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour
// vertex 3
	0.075f, 0.0f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour

	// triangle 3
// vertex 1
	-0.075f, 0.0f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour
// vertex 2
	0.075f, 0.0f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
// vertex 3
	-0.05f, -0.05f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
	
	// triangle 4
// vertex 1
	0.05f, -0.05f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
// vertex 2
	0.075f, 0.0f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
// vertex 3
	-0.05f, -0.05f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
	
	// circle centre (starting vertex)
	0.0f, 0.0f, 0.0f,	//position
	1.0f, 0.0f, 0.0f,	//colour
};


GLuint g_VBO = 0;				// vertex buffer object identifier
GLuint g_VAO = 0;				// vertex array object identifier
GLuint g_shaderProgramID = 0;	// shader program identifier
GLuint g_modelMatrixIndex = 0;	// location in shader
glm::mat4 g_modelMatrix[MODEL_MATRICES];		// object's model matrix
bool g_anim_toggle = true; // toggles animation 
bool g_dir_toggle = true; // toggles direction of rotation

void generate_circle()
{
	float slice_angle = PI * 2 / static_cast<float>(32);	// angle for each fan slice
	float angle = slice_angle;								// angle used to generate x and y coordinates
	g_vertices[18].position[0] = CIRCLE_RADIUS;	// set x coordinate of vertex 18 (circle vertex 1)

	// generate vertex coordinates for triangle fan
	for (int i = 19; i < 49 + 2; i++) // 49 because 19 starting vertex + 32 vertices in circle
	{
		g_vertices[i].position[0] = CIRCLE_RADIUS * cos(angle); // x position
		g_vertices[i].position[1] = CIRCLE_RADIUS * sin(angle); // y position
		g_vertices[i].position[2] = 0.0f; // z position (always 0.0)
		g_vertices[i].color[0] = 1.0f; // color (always red)
		g_vertices[i].color[1] = 0.0f;
		g_vertices[i].color[2] = 0.0f;

		angle += slice_angle; // update to next angle
	}
}

static void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// set clear background colour

	// create and compile our GLSL program from the shader files
	g_shaderProgramID = loadShaders("VS.vert", "FS.frag");

	// generate vertices of triangle fan
	generate_circle();

	// find the location of shader variables
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint colorIndex = glGetAttribLocation(g_shaderProgramID, "aColor");
	g_modelMatrixIndex = glGetUniformLocation(g_shaderProgramID, "uModelMatrix");

	
	float angle = 0.0f;
	// initialise model matrix 0 and 1 to the identity matrix
	g_modelMatrix[0] = glm::mat4(1.0f); // Wheel
	g_modelMatrix[1] = glm::mat4(1.0f); // Base
	for (int i = 2; i < MODEL_MATRICES; i++)
	{
		g_modelMatrix[i] = glm::mat4(1.0f); // Carriage i identity matrix initialization
		GLfloat xpos = CIRCLE_RADIUS * cos(angle + i / 1.9); // calculate carriage x position
		GLfloat ypos = CIRCLE_RADIUS * sin(angle + i / 1.9); // calculate carriage y position
		g_modelMatrix[i] *= glm::translate(glm::vec3(xpos, ypos, 0.0f)); // apply transformations to Carriages
	}

	// generate identifier for VBO and copy data to GPU
	glGenBuffers(1, &g_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);

	// generate identifier for VAO
	glGenVertexArrays(1, &g_VAO);

	// create VAO and specify VBO data
	glBindVertexArray(g_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	// interleaved attributes
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

	glEnableVertexAttribArray(positionIndex);	// enable vertex attributes
	glEnableVertexAttribArray(colorIndex);
}

static void update_scene()
{
	static float angle = 0.0f;
	static float xpos, ypos;
	static bool swingdir = true; 
	static float swingangle = 0.0f;
	
	// Change direction of swing if angle gets too high or low
	if (swingangle > 1.5)
	{
		swingdir = true;
	}
	else if (swingangle < -1.5)
	{
		swingdir = false;
	}
	
	// Depending on the direction of swing, increment angle up or down
	if (swingdir == true) 
	{
		swingangle -= 0.1;
	}
	else
	{
		swingangle += 0.1;
	}

	if (g_anim_toggle == true) // if animation is toggled on
	{
		if (g_dir_toggle == true) // check rotation toggle
		{
			angle += ANIMATION_SPEED; // Rotate left
		}
		else
		{
			angle -= ANIMATION_SPEED; // Rotate right
		}
		// Wheel animation
		g_modelMatrix[0] = glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)); // apply transformation
		// Carriage animation
		for (int i = 2; i < MODEL_MATRICES; i++) // for each carriage
		{
			xpos = CIRCLE_RADIUS * cos(angle + i / 1.9); // calculate x position of carriage i
			ypos = CIRCLE_RADIUS * sin(angle + i / 1.9); // calculate y position of carriage i
			g_modelMatrix[i] = glm::translate(glm::vec3(xpos, ypos, 0.0f)) // Apply transformations to carriage i
				* glm::rotate((swingangle/i), glm::vec3(0.0f, 0.0f, 1.0f)); // Diversify angle of swing by dividing by i
		}
	}
}

// function used to render the scene
static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);	// clear colour buffer

	glUseProgram(g_shaderProgramID);	// use the shaders associated with the shader program

	glBindVertexArray(g_VAO);		// make VAO active

	// set uniform model transformation matrix
	glUniformMatrix4fv(g_modelMatrixIndex, 1, GL_FALSE, &g_modelMatrix[0][0][0]);
	// Wheel
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Render as lines
	glDrawArrays(GL_TRIANGLE_FAN, 17, 34);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Render as fill below this line

	for (int i = 2; i < MODEL_MATRICES; i++)
	{
		// set uniform model transformation matrix
		glUniformMatrix4fv(g_modelMatrixIndex, 1, GL_FALSE, &g_modelMatrix[i][0][0]);
		// Carriage i
		glDrawArrays(GL_TRIANGLE_STRIP, 5, 12);	// display the vertices based on the primitive type
	}

	// set uniform model transformation matrix
	glUniformMatrix4fv(g_modelMatrixIndex, 1, GL_FALSE, &g_modelMatrix[1][0][0]);
	// Base
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_TRIANGLES, 2, 3); 	// display the vertices based on the primitive type

	glFlush();	// flush the pipeline
}

// key press or release callback function
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// quit if the ESCAPE key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		// set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	// toggle animation if P key is pressed
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (g_anim_toggle == true) // animation is on - turn off
		{
			g_anim_toggle = false;
		}
		else // animation is off - turn on
		{
			g_anim_toggle = true;
		}
	}
	// toggle direction of rotation if D key is pressed
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		if (g_dir_toggle == true) // direction is left - make right
		{
			g_dir_toggle = false;
		}
		else // animation is right - make left
		{
			g_dir_toggle = true;
		}
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
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 1 - Part 1", NULL, NULL);

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
	init();

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene();				// update the scene
		render_scene();				// render the scene

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events
	}

	// clean up
	glDeleteProgram(g_shaderProgramID);
	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

