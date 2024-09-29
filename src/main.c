#include <stdio.h>

#include <epoxy/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "util.h"

#define WINDOW_W 800
#define WINDOW_H 600


GLFWwindow* init() {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		printf("Could not initialize glfw.\n");
		return NULL;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Hello World", NULL, NULL);
	if (!window) {
		printf("Could not create a glfw window.\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	return window;
}

int main(void) {
	GLFWwindow* window = init();

	double mouseX, mouseY;

	unsigned int shaderProg = MakeShaderProgram("shaders/main.vert", "shaders/main.frag");
	int mLoc = glGetUniformLocation(shaderProg, "m");
	int vLoc = glGetUniformLocation(shaderProg, "v");
	int pLoc = glGetUniformLocation(shaderProg, "p");
	int mouseLoc = glGetUniformLocation(shaderProg, "Mouse");
	int winSizeLoc = glGetUniformLocation(shaderProg, "WinSize");

	// Model* model = LoadModel("res/plane.obj");
	Model* model = LoadModel("res/mech.glb");

	mat4 p, v;
	glm_mat4_identity(v);
	glm_translate(v, (vec3){0, -6, -15});
	glm_perspective(glm_rad(45), WINDOW_W / WINDOW_H, 0.1f, 100.f, p);

	vec3 light = {10, 10, 0};


	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	float time = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		float delta = glfwGetTime() - time;
		time = glfwGetTime();
		glfwGetCursorPos(window, &mouseX, &mouseY);

		/* Render here */
		glClearColor(0.f, 0.2f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProg);

		vec3 transVec;
		glm_vec3_scale((vec3){0, -1, 0}, delta / 10, transVec);
		glm_rotate(model->transform, delta/5, GLM_YUP);

		glUniformMatrix4fv(mLoc, 1, GL_FALSE, (float*)model->transform);
		glUniformMatrix4fv(vLoc, 1, GL_FALSE, (float*)v);
		glUniformMatrix4fv(pLoc, 1, GL_FALSE, (float*)p);
		glUniform2f(mouseLoc, mouseX, -mouseY + WINDOW_H);
		glUniform2f(winSizeLoc, WINDOW_W, WINDOW_H);

		glBindVertexArray(model->vao);
		glDrawElements(GL_TRIANGLES, model->elementsCount, GL_UNSIGNED_INT, 0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}