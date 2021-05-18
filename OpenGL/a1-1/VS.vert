#version 330 core

// input data (different for all executions of this shader)
in vec3 aPosition;
in vec3 aColor;

// model space matrix
uniform mat4 uModelMatrix;

// output data (will be interpolated for each fragment)
out vec3 vColor;

void main()
{
	// set vertex position
    gl_Position = uModelMatrix * vec4(aPosition, 1.0);

	// the color of each vertex will be interpolated
	// to produce the color of each fragment
	vColor = aColor;
}