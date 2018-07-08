#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <windows.h>
#include <stdio.h>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "stb_image.h"
#include <math.h>
#include "Board.h"


int window;
GLuint *textures;
Board* b;
double trueX, trueY;
int windowWidth = 1600, windowHeight = 900;
double halfWidth = windowWidth / 2, halfHeight = windowHeight / 2;
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

bool withinBoard(int x, int y)
{
	return x >= boardLeft && x <= boardRight && y <= boardTop && y >= boardBottom;
}

void updateBoardIncrements()
{
	boardIncrementX = (boardRight - boardLeft) / (b->width);
	boardIncrementY = (boardTop - boardBottom) / (b->height);
}

void render()
{
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.2, 0.2, 0.2, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	for (int row = 0; row < b->height; row++)
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
			else if (b->isFlagged(row, col))
			{
				text = 11;
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
	glutSwapBuffers();
}

void resize(int w, int h)
{
	windowWidth = w;
	halfWidth = w / 2;
	windowHeight = h;
	halfHeight = h / 2;
}

void mouseCallback(int button, int state, int x, int y)
{
	trueX = (x / halfWidth) - 1;
	trueY = 1 - (y / halfHeight);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (withinBoard(trueX, trueY))
		{
			int r = (int)floor((boardTop - trueY) / boardIncrementY);
			int c = (int)floor((trueX - boardLeft) / boardIncrementX);
			b->click(r, c);
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (withinBoard(trueX, trueY))
		{
			int r = (int)floor((boardTop - trueY) / boardIncrementY);
			int c = (int)floor((trueX - boardLeft) / boardIncrementX);
			b->changeFlag(r, c);
		}
	}
}

void keyboardCallback(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	window = glutCreateWindow("Minesweeper");

	textures = new GLuint[12];
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
	loadTexture(textures[11], "assets/flagged.png");

	b = new Board(10, 10, 20);
	updateBoardIncrements();

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutIdleFunc(render);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);

	glutMainLoop();


	delete textures;
	exit(EXIT_SUCCESS);
	return 0;
}