//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "GL\glew.h"
#include "GL\freeglut.h"
#include "stb_image.h"

#include "Board.h"
#include "Button.h"


int window;
GLuint* textures;
std::vector<Button> buttons;
Board* b;
double trueX, trueY;
int windowWidth = 1600, windowHeight = 900;
double halfWidth = windowWidth / 2, halfHeight = windowHeight / 2;

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

void drawBoard()
{
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
			glVertex2d(b->left + b->incrementX * col,
				b->top - b->incrementY * (row + 1));

			glTexCoord2d(0, 0);
			glVertex2d(b->left + b->incrementX * col,
				b->top - b->incrementY * row);

			glTexCoord2d(1, 0);
			glVertex2d(b->left + b->incrementX * (col + 1),
				b->top - b->incrementY * row);

			glTexCoord2d(1, 1);
			glVertex2d(b->left + b->incrementX * (col + 1),
				b->top - b->incrementY * (row + 1));
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glDisable(GL_TEXTURE_2D);
}

void drawButtons()
{
	for (int button = 0; button < buttons.size(); button++)
	{
		Button curr = buttons.at(button);
		glColor3d(1, 0, 0);
		glBegin(GL_QUADS);
		glVertex2d(curr.x, curr.y + curr.height);
		glVertex2d(curr.x, curr.y);
		glVertex2d(curr.x + curr.width, curr.y);
		glVertex2d(curr.x + curr.width, curr.y + curr.height);
		glEnd();
		glColor3d(1, 1, 1);
	}
}

void render()
{
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.2, 0.2, 0.2, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	drawBoard();
	drawButtons();

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

void newGameCallback()
{
	b = new Board(10, 10, 20);
}

void mouseCallback(int button, int state, int x, int y)
{
	trueX = (x / halfWidth) - 1;
	trueY = 1 - (y / halfHeight);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (b->withinBoard(trueX, trueY))
		{
			int r = (int)floor((b->top - trueY) / b->incrementY);
			int c = (int)floor((trueX - b->left) / b->incrementX);
			b->click(r, c);
		}
		for (int button = 0; button < buttons.size(); button++)
		{
			if (buttons.at(button).withinButton(trueX, trueY))
			{
				buttons.at(button).click();
			}
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (b->withinBoard(trueX, trueY))
		{
			int r = (int)floor((b->top - trueY) / b->incrementY);
			int c = (int)floor((trueX - b->left) / b->incrementX);
			b->changeFlag(r, c);
			std::cout << b->displayedMinesRemaining << std::endl;
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
	buttons.push_back(Button(0.2, 0.1, 0.7, 0, newGameCallback));


	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutIdleFunc(render);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);

	glutMainLoop();


	delete textures;
	delete b;
	exit(EXIT_SUCCESS);
	return 0;
}