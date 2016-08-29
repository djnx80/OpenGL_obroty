#include "stdafx.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <iostream> 
#include <fstream>
#include <string>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/glm.hpp>
//#define GLM_FORCE_RADIANS

using namespace std;


int window_height = 600;
int window_width = 800;

void window_size_callback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
}
static void error_callback(int error, const char* description)
{
	std::cerr << "Error: " << description;
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
GLint loadShaders(string vertex_shader, string fragment_shader) {
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	string vertex_shader_data;
	ifstream vertex_shader_file(vertex_shader.c_str(), ios::in);
	if (vertex_shader_file.is_open())
	{
		string line;
		while (getline(vertex_shader_file, line))
			vertex_shader_data += "\n" + line;
		vertex_shader_file.close();
		cout << "odczytano plik vertex" << endl;
	}
	else
	{
		cout << "nie ma pliku vertex" << endl;
	}
	string fragment_shader_data;
	ifstream fragment_shader_file(fragment_shader.c_str(), ios::in);
	if (fragment_shader_file.is_open())
	{
		string line;
		while (getline(fragment_shader_file, line))
			fragment_shader_data += "\n" + line;
		fragment_shader_file.close();
		cout << "odczytano plik fragment" << endl;
	}
	else
	{
		cout << "nie ma pliku fragment" << endl;
	}

	const char * vertex_ptr = vertex_shader_data.c_str();
	const char * fragment_ptr = fragment_shader_data.c_str();
	glShaderSource(vertex_shader_id, 1, &vertex_ptr, NULL);
	glShaderSource(fragment_shader_id, 1, &fragment_ptr, NULL);
	glCompileShader(vertex_shader_id);
	glCompileShader(fragment_shader_id);
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, vertex_shader_id);
	glAttachShader(shader_programme, fragment_shader_id);
	glLinkProgram(shader_programme);
	int link_status = -1;
	glGetProgramiv(shader_programme, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE) {
		cout << "Shader zle sie zaaplikowal" << endl;

		const int max_length = 2048;
		int length = 0;
		char log_text[max_length];
		glGetShaderInfoLog(vertex_shader_id, max_length, &length, log_text);
		cout << log_text<<endl;

		return -1;
	}
	else
	{
		cout << " Shader prawidlowo zaaplikowany" << endl;
	}
	int vertex_shader_status = -1;
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &vertex_shader_status);
	if (vertex_shader_status != GL_TRUE) {
		cout << "Cos nie posz³o z kompilacja vertex" << endl;
		return -1;
	}
	else {
		cout << "Kompilacja vertex sie udala" << endl;
	}
	int fragment_shader_status = -1;
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &fragment_shader_status);
	if (fragment_shader_status != GL_TRUE) {
		cout << "Cos nie posz³o z kompilacja fragment" << endl;
		return -1;
	}
	else {
		cout << "Kompilacja fragment sie udala" << endl;
	}
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	return shader_programme;
}


int main(void)
{


	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return 1;

	glfwWindowHint(GLFW_SAMPLES, 4);		// wyg³adzanie krawêdzi
	GLFWwindow* window = glfwCreateWindow(800, 600, "My GLFW Window", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	GLenum error_code = glewInit();
	if (error_code != GLEW_OK) {
		std::cerr << "GLEW init error: " << glewGetErrorString(error_code);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	GLfloat points[] =
	{ -0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f };

	GLfloat colours[] =
	{ 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, };

	GLuint position_vbo = 0;
	glGenBuffers(1, &position_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint colour_vbo = 0;
	glGenBuffers(1, &colour_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colour_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colour_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint shaders = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");

	GLint macierz_prz = glGetUniformLocation(shaders, "macierz_przesuniecia");
	if (macierz_prz == -1) 
		cout << "nie znalaziono zmiennej macierz_przesuniecia" << endl;
	else { cout << "zmienna znaleziona" << endl; }


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	glm::mat4 translate_matrix;
	glm::mat4 rotate_matrix;
	const float radius = 0.5;
	float x_trans = 0.0; 
	float y_trans = 0.0; 
	float angle = 0;



	while (!glfwWindowShouldClose(window))
	{

		static double prev_time = glfwGetTime(); 
		double actual_time = glfwGetTime();
		double elapsed_time = actual_time - prev_time; 
		if (elapsed_time > 0.01) 
		{ 
			prev_time = actual_time;
			angle += 1; 
			x_trans = radius * cos(angle * 3.14 / 180.0);
			y_trans = radius * sin(angle * 3.14 / 180.0);
		}

		translate_matrix = glm::translate(glm::mat4(1.0), glm::vec3(x_trans, y_trans, 0.0));
		rotate_matrix = glm::rotate(translate_matrix, angle/15, glm::vec3(0, 1, 0));


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, window_width, window_height);

		glUseProgram(shaders);
		glUniformMatrix4fv(macierz_prz, 1, GL_FALSE, glm::value_ptr(rotate_matrix));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
