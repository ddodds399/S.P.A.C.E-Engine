
#include <windows.h>
#include <C:\OpenGl Libraries\glew\glew-1.9.0\include\GL\glew.h>;
#include <C:\OpenGl Libraries\glfw3\include\GLFW\glfw3.h>
#include <stdio.h>  
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <cmath>
#include <cstdio>
#include <C:\Users\Thomas\Documents\Visual Studio 2015\Projects\Towers\Towers\glm-0.9.7.1\glm\glm.hpp>
#include <C:\Users\Thomas\Documents\Visual Studio 2015\Projects\Towers\Towers\glm-0.9.7.1\glm\gtc\matrix_transform.hpp>
using namespace glm;
#include<C:\Users\Thomas\Documents\Visual Studio 2015\Projects\Towers\Towers\SOIL\SOIL.h>

GLint x = 20, y = -3, z = -50;

int Texture() {

	GLint tex = SOIL_load_OGL_texture("uvtemplate.bmp", 256, 0,SOIL_FLAG_TEXTURE_REPEATS);
	return tex;
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {
	static char VertexShaderCode[9128];
	static char FragmentShaderCode[9128];

	int i;
	FILE *VertexShaderStream;
	FILE *FragmentShaderStream;
	GLint Result = GL_FALSE;
	int InfoLogLength;
	char const *SourcePointer;
	GLint VertexShaderID;
	GLint FragmentShaderID;
	GLint ProgramID;

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	if ((VertexShaderStream = fopen(vertex_file_path, "rb")) != NULL) {
		i = 0; while (fread(&(VertexShaderCode[i]), 1, 1, VertexShaderStream) == 1)i++;
	}
	else {
		printf("Fail to open%s.\n", vertex_file_path);
		return 0;
	}
	fclose(VertexShaderStream);

	// Read the Fragment Shader code from the file
	if ((FragmentShaderStream = fopen(fragment_file_path, "rb")) != NULL) {
		i = 0; while (fread(&(FragmentShaderCode[i]), 1, 1, FragmentShaderStream) == 1)i++;
	}
	else {
		printf("Fail to open%s.\n", fragment_file_path);
		return 0;
	}
	fclose(FragmentShaderStream);

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	SourcePointer = &VertexShaderCode[0];
	glShaderSource(VertexShaderID, 1, &SourcePointer, NULL);
	glCompileShader(VertexShaderID);
	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		char VertexShaderErrorMessage[1024];
		printf("Info (v)\n");
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("[%s]\n", VertexShaderErrorMessage);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	SourcePointer = &FragmentShaderCode[0];
	glShaderSource(FragmentShaderID, 1, &SourcePointer, NULL);
	glCompileShader(FragmentShaderID);
	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		char FragmentShaderErrorMessage[1024];
		printf("Info (f)\n");
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("[%s]\n", FragmentShaderErrorMessage);
	}

	// Link the program
	printf("Linking program\n");
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		char  ProgramErrorMessage[1024];
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", ProgramErrorMessage);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	if (key == GLFW_KEY_UP) {
		y--;
	}
	if (key == GLFW_KEY_DOWN) {
		y++;
	}
	if (key == GLFW_KEY_LEFT) {
		x++;
	}
	if (key == GLFW_KEY_RIGHT) {
		x--;
	}
	if (key == GLFW_KEY_S) {
		z--;
	}
	if (key == GLFW_KEY_W) {
		z++;
	}

}

GLFWwindow* initWindow(const int resX, const int resY)
{


	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing



									 // Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(resX, resY, "USE ARROW KEYS TO MOVE LEFT, RIGHT, UP, DOWN AND 'W' AND 'S' TO MOVE BACKWARDS AND FORWARDS", NULL, NULL);

	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();

		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, controls);

	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST); // Depth Testing
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	return window;
}

void drawCube()
{
	GLfloat vertices1[] =
	{
		-10, -1, -10,   -10, -1,  1,   -10,  1,  1,   -10,  1, -10,
		1, -1, -10,    1, -1,  1,    1,  1,  1,    1,  1, -10,
		-10, -1, -10,   -10, -1,  1,    1, -1,  1,    1, -1, -10,
		-10,  1, -10,   -10,  1,  1,    1,  1,  1,    1,  1, -10,
		-10, -1, -10,   -10,  1, -10,    1,  1, -10,    1, -1, -10,
		-10, -1,  1,   -10,  1,  1,    1,  1,  1,    1, -1,  1
	};

	GLfloat colors1[] =
	{
		0, 0, 0,   0, 0, 1,   0, 1, 1,   0, 1, 0,
		1, 0, 0,   1, 0, 1,   1, 1, 1,   1, 1, 0,
		0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,
		0, 1, 0,   0, 1, 1,   1, 1, 1,   1, 1, 0,
		0, 0, 0,   0, 1, 0,   1, 1, 0,   1, 0, 0,
		0, 0, 1,   0, 1, 1,   1, 1, 1,   1, 0, 1
	};
	GLfloat vertices2[] =
	{
		2, 2, 2,   2, 2, 4,   2, 4, 4,  2, 4, 2,
		4, 2, 2,   4, 2, 4,   4, 4, 4,  4, 4, 2,
		2, 2, 2,   2, 2, 4,   4, 2, 4,  4, 2, 2,
		2, 4, 2,   2, 4, 4,   4, 4, 4,  4, 4, 2,
		2, 2, 2,   2, 4, 2,   4, 4, 2,  4, 2, 2,
		2, 2, 4,   2, 4, 4,   4, 4, 4,  4, 2, 4
	};

	GLfloat colors2[] =
	{
		0, 0, 0,   0, 0, 1,   0, 1, 1,   0, 1, 0,
		1, 0, 0,   1, 0, 1,   1, 1, 1,   1, 1, 0,
		0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,
		0, 1, 0,   0, 1, 1,   1, 1, 1,   1, 1, 0,
		0, 0, 0,   0, 1, 0,   1, 1, 0,   1, 0, 0,
		0, 0, 1,   0, 1, 1,   1, 1, 1,   1, 0, 1
	};
	static float alpha = 0;

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	GLuint ProgramID = LoadShaders("shader.vert", "shader.frag");
	GLuint MatrixID = glGetUniformLocation(ProgramID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	//glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
		
	GLint texture = Texture();
	GLint TextureID = glGetUniformLocation(ProgramID, "TextureSampler1");
	GLuint texturebuffer, vertexBuffer, colourbuffer;

	/**GLfloat textBuffData[] =
	{
		0, 0,   1, 0,   1, 1,  0, 1,
		0, 0,   1, 0,   1, 1,  0, 1,
		0, 0,   1, 0,   1, 1,  0, 1,
		0, 0,   1, 0,   1, 1,  0, 1,
		0, 0,   1, 0,   1, 1,  0, 1,
		0, 0,   1, 0,   1, 1,  0, 1,

	};*/

	static const GLfloat g_uv_buffer_data[] = {
		0.000059f, 1.0f - 0.000004f,
		0.000103f, 1.0f - 0.336048f,
		0.335973f, 1.0f - 0.335903f,
		1.000023f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.336024f, 1.0f - 0.671877f,
		0.667969f, 1.0f - 0.671889f,
		1.000023f, 1.0f - 0.000013f,
		0.668104f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.000059f, 1.0f - 0.000004f,
		0.335973f, 1.0f - 0.335903f,
		0.336098f, 1.0f - 0.000071f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.336024f, 1.0f - 0.671877f,
		1.000004f, 1.0f - 0.671847f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.668104f, 1.0f - 0.000013f,
		0.335973f, 1.0f - 0.335903f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.668104f, 1.0f - 0.000013f,
		0.336098f, 1.0f - 0.000071f,
		0.000103f, 1.0f - 0.336048f,
		0.000004f, 1.0f - 0.671870f,
		0.336024f, 1.0f - 0.671877f,
		0.000103f, 1.0f - 0.336048f,
		0.336024f, 1.0f - 0.671877f,
		0.335973f, 1.0f - 0.335903f,
		0.667969f, 1.0f - 0.671889f,
		1.000004f, 1.0f - 0.671847f,
		0.667979f, 1.0f - 0.335851f
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STATIC_DRAW);

	glUseProgram(ProgramID);

	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(TextureID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
		
	/* We have a color array and a vertex array */
	/**glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_2D_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices1);
	glColorPointer(3, GL_FLOAT, 0, colors1);
	glTexCoordPointer(3, GL_FLOAT, 0, textBuffData);

	/* Send data : 24 vertices */
	glDrawArrays(GL_QUADS, 0, 24);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//glVertexPointer(3, GL_FLOAT, 0, vertices2);
	//glColorPointer(3, GL_FLOAT, 0, colors2);

	/* Send data : 24 vertices */
	//glDrawArrays(GL_QUADS, 0, 24);

	/* Cleanup states */
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	alpha += 1;
	glEnd();
	
}

void drawCylinder(float radius, float height, float slices, float stacks) {

	gluCylinder(gluNewQuadric(), radius, radius, height, slices, stacks);
}


void DrawCircle(float cx, float cy, float r, int num_segments)
{
	glPolygonMode(GL_LINE_LOOP,GL_FILL);
	glBegin(GL_POLYGON);
	for (int ii = 0; ii < num_segments; ii++){
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 

		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 

		glVertex2f(x + cx, y + cy);//output vertex 
	}
	glEnd();
	
}

void resetMatrix(int windowWidth, int windowHeight) {
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	gluPerspective(60, (double)windowWidth / (double)windowHeight, 0.1, 100);

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glTranslatef(x, y, z);
}

void display(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		// Scale to window size
		GLint windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);

		// Draw stuff
		glClearColor(0.0, 0.8, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		resetMatrix(windowWidth, windowHeight);
		glScalef(4, 1, 1);
		
		
		drawCube();

		glUseProgram(0);


		resetMatrix(windowWidth, windowHeight);
		glTranslatef(-18, 11, -5);
		glRotatef(90, 1, 0, 0);
		glColor3f(1,0,0);
		drawCylinder(0.5, 10, 100, 10);


		resetMatrix(windowWidth, windowHeight);
		glTranslatef(-3, 11, -5);
		glRotatef(90, 1, 0, 0);
		glColor3f(1, 1, 0);
		drawCylinder(0.5, 10, 100, 10);

		resetMatrix(windowWidth, windowHeight);
		glTranslatef(-32, 11, -5);
		glRotatef(90, 1, 0, 0);
		glColor3f(0,0,1);
		drawCylinder(0.5, 10, 100, 10);

		resetMatrix(windowWidth, windowHeight);
		glTranslatef(10, 10, 10);
		//glColor3f(1, 0.43, 0.78);
		glColor3f(1, 0, 0);

		DrawCircle(10,10, 10, 100);
		resetMatrix(windowWidth, windowHeight);
		static float beta = 0;
		glRotatef(beta, 0, 1, 0);
		beta += 0.2;
		glTranslatef(8, 0, 0);


		
		

		
		
		
		// Update Screen
		glfwSwapBuffers(window);

		// Check for any input, or window movement
		glfwPollEvents();
	}
}

int main(int argc, char** argv)
{
	GLFWwindow* window = initWindow(1024, 620);
	if (NULL != window)
	{
		GLenum err = glewInit();
		display(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

