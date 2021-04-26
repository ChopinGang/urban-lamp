#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "../glad/include/glad/glad.h"
#include "../glfw/GLFW/glfw3.h"
#include <string.h>

GLFWwindow* window;

int main() {
	
	if (!glfwInit()) {
		printf("lol no glfw");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	window = glfwCreateWindow(1024, 768, "gam", NULL, NULL);
	if (window == NULL) {
		printf("error wit da glfw lmao");
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("failed to init glad");
		return -1;
	}
	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint vaid;
	glGenVertexArrays(1, &vaid);
	glBindVertexArray(vaid);

	static const GLfloat gVertexBufferData[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

	GLuint programID = loadShaders();

	do {
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programID);
		//

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		//
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	
}

GLuint loadShaders() {
	printf("loading shaders");
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	char* buffer = 0;
	char* bufferf = 0;
	long lengthf;
	long length;
	FILE* file = fopen("main/vertex.sh", "rb");
	FILE* filef = fopen("main/fragment.sh", "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_END);
		buffer = malloc(length + 1);
		if (buffer) {
			fread(buffer, 1, length, file);
		}
		fclose(file);
		buffer[length] = '\0';
	}
	if (filef) {
		fseek(filef, 0, SEEK_END);
		lengthf = ftell(filef);
		fseek(filef, 0, SEEK_END);
		bufferf = malloc(lengthf + 1);
		if (bufferf) {
			fread(bufferf, 1, lengthf, filef);
		}
		fclose(filef);
		bufferf[lengthf] = '\0';
	}

	if (buffer && bufferf) {
		GLint result = GL_FALSE;
		int infoLogLength;

		printf("Compiling shader : vertex");
		glShaderSource(vertexShaderID, 1, &buffer, NULL);
		glCompileShader(vertexShaderID);
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			char infoLog[512];
			glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, infoLog);
			printf("%s\n", infoLog);
		}

		printf("Compiling shader : frag");
		glShaderSource(fragmentShaderID, 1, bufferf, NULL);
		glCompileShader(fragmentShaderID);
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			char infoLog[512];
			glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, infoLog);
			printf("%s\n", infoLog);
		}

		printf("Linking program\n");
		GLuint programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			char infoLog[512];
			glGetProgramInfoLog(programID, infoLogLength, NULL, infoLog);
			printf("%s\n", infoLog);
		}

		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		return programID;
	} else
		return -1;
}
