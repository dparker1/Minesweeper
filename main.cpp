#pragma comment(lib, "OpenGL32.lib")
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <windows.h>
#include <stdio.h>
#include <GL\GL.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <math.h>
#include "Board.h"


GLFWwindow* window;
GLuint *textures;
Board* b;
double cursorX, cursorY, trueX, trueY;
int windowWidth = 1920, windowHeight = 1080;
int halfWidth = windowWidth / 2, halfHeight = windowHeight / 2;
double boardTop = 0.95,
	boardBottom = -0.95,
	boardLeft = -0.95,
	boardRight = 0.6;
double boardIncrementX, boardIncrementY;

void loadTexture(GLuint texture, const char* filename)
{
	int width, height, nrChan;
	unsigned char *data;

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	data = stbi_load(filename, &width, &height, &nrChan, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &cursorX, &cursorY);
		trueX = (cursorX / halfWidth) - 1;
		trueY = 1 - (cursorY / halfHeight);
		if (trueX >= boardLeft && trueX <= boardRight && trueY <= boardTop && trueY >= boardBottom)
		{
			int r = (int)floor((boardTop - trueY) / boardIncrementY);
			int c = (int)floor((trueX - boardLeft) / boardIncrementX);
			b->click(r, c);
		}
		
	}
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(int argc, char** argv)
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(windowWidth, windowHeight, "Minesweeper", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetMouseButtonCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwMakeContextCurrent(window);

	textures = new GLuint[11];
	glGenTextures(11, textures);
	loadTexture(textures[0], "assets/0.png");
	loadTexture(textures[1], "assets/1.png");
	loadTexture(textures[2], "assets/2.png");
	loadTexture(textures[3], "assets/3.png");
	loadTexture(textures[4], "assets/4.png");
	loadTexture(textures[5], "assets/5.png");
	loadTexture(textures[6], "assets/6.png");
	loadTexture(textures[7], "assets/7.png");
	loadTexture(textures[8], "assets/8.png");
	loadTexture(textures[9], "assets/unrevealed.png");
	loadTexture(textures[10], "assets/mine.png");

	b = new Board(10, 10, 10);

	boardIncrementX = (boardRight - boardLeft) / (b->width);
	boardIncrementY = (boardTop - boardBottom) / (b->height);
	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		for (int row = 0;row < b->height; row++)
		{
			for (int col = 0; col < b->width; col++)
			{
				int text = 9;
				if (b->isRevealed(row, col))
				{
					text = b->mines(row, col);
					if (b->isMined(row, col))
					{
						text = 10;
					}
				}

				glBindTexture(GL_TEXTURE_2D, textures[text]);
				glBegin(GL_QUADS);
				glTexCoord2d(0, 1);
				glVertex2d(boardLeft + boardIncrementX * col,
					boardTop - boardIncrementY * (row + 1));

				glTexCoord2d(0, 0); 
				glVertex2d(boardLeft + boardIncrementX * col,
					boardTop - boardIncrementY * row);

				glTexCoord2d(1, 0); 
				glVertex2d(boardLeft + boardIncrementX * (col + 1),
					boardTop - boardIncrementY * row);

				glTexCoord2d(1, 1); 
				glVertex2d(boardLeft + boardIncrementX * (col + 1),
					boardTop - boardIncrementY * (row + 1));
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete textures;
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	return 0;
}