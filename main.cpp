#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "GL\glew.h"
#include "GL\freeglut.h"
#include "stb_image.h"

#include "Board.h"
#include "Button.h"


int window;
GLuint* textures;
std::vector<Button> buttons;
Board* b;
bool leftClick, rightClick;
double trueX, trueY;
int windowWidth = 1600, windowHeight = 900;
double halfWidth = windowWidth / 2, halfHeight = windowHeight / 2;
double digitalLength = 0.03, digitalWidth = 0.005;

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

void drawBoxVertices(double x, double y, double w, double h)
{
	glVertex2d(x, y - h);
	glVertex2d(x, y);
	glVertex2d(x + w, y);
	glVertex2d(x + w, y - h);
}

void drawButtons()
{
	for (int button = 0; button < buttons.size(); button++)
	{
		Button curr = buttons.at(button);
		glBegin(GL_QUADS);
		glVertex2d(curr.left, curr.bottom);
		glVertex2d(curr.left, curr.top);
		glVertex2d(curr.right, curr.top);
		glVertex2d(curr.right, curr.bottom);
		glEnd();
		glColor3d(1, 1, 1);
	}
}

void drawStateBox()
{
	switch (b->state)
	{
	case 2:
		glColor3d(1, 0.5, 0);
		break;
	case 1:
		glColor3d(1, 1, 0);
		break;
	case 0:
		glColor3d(0, 1, 0);
		break;
	case -1:
		glColor3d(1, 0, 0);
		break;
	}
	glBegin(GL_QUADS);
	drawBoxVertices(0.75, 0.5, 0.04, 0.05);
	glEnd();
	glColor3d(1, 1, 1);
}

/*
l1 - top horizontal
l2 - top right vertical
l3 - bottom right vertical
l4 - bottom horizontal
l5 - bottom left vertical
l6 - top left vertical
l7 - middle horizontal
*/
void drawDigitalLegs(double x, double y, bool l1, bool l2, bool l3, bool l4, bool l5, bool l6, bool l7)
{
	if (l1)
	{
		drawBoxVertices(x + digitalWidth, y, digitalLength - digitalWidth, digitalWidth);
	}
	if (l2)
	{
		drawBoxVertices(x + digitalLength, y, digitalWidth, digitalLength);
	}
	if (l3)
	{
		drawBoxVertices(x + digitalLength, y - digitalLength, digitalWidth, digitalLength);
	}
	if (l4)
	{
		drawBoxVertices(x + digitalWidth, y - (2 * digitalLength) + digitalWidth, digitalLength - digitalWidth, digitalWidth);
	}
	if (l5)
	{
		drawBoxVertices(x, y - digitalLength, digitalWidth, digitalLength);
	}
	if (l6)
	{
		drawBoxVertices(x, y, digitalWidth, digitalLength);
	}
	if (l7)
	{
		drawBoxVertices(x + digitalWidth, y - digitalLength, digitalLength - digitalWidth, digitalWidth);
	}
}

void drawDigitalNumber(int n, double x, double y)
{
	glColor3d(1, 0, 0);
	glBegin(GL_QUADS);
	switch (n)
	{
	case 0:
		drawDigitalLegs(x, y, 1, 1, 1, 1, 1, 1, 0);
		break;
	case 1:
		drawDigitalLegs(x, y, 0, 1, 1, 0, 0, 0, 0);
		break;
	case 2:
		drawDigitalLegs(x, y, 1, 1, 0, 1, 1, 0, 1);
		break;
	case 3:
		drawDigitalLegs(x, y, 1, 1, 1, 1, 0, 0, 1);
		break;
	case 4:
		drawDigitalLegs(x, y, 0, 1, 1, 0, 0, 1, 1);
		break;
	case 5:
		drawDigitalLegs(x, y, 1, 0, 1, 1, 0, 1, 1);
		break;
	case 6:
		drawDigitalLegs(x, y, 1, 0, 1, 1, 1, 1, 1);
		break;
	case 7:
		drawDigitalLegs(x, y, 1, 1, 1, 0, 0, 0, 0);
		break;
	case 8:
		drawDigitalLegs(x, y, 1, 1, 1, 1, 1, 1, 1);
		break;
	case 9:
		drawDigitalLegs(x, y, 1, 1, 1, 1, 0, 1, 1);
		break;
	}
	glEnd();
	glColor3d(1, 1, 1);
}

void drawGameState()
{
	glColor3d(0, 0, 0);
	glBegin(GL_QUADS);
	drawBoxVertices(0.65, 0.5, digitalLength * 3, digitalLength * 2);
	drawBoxVertices(0.8, 0.5, digitalLength * 3, digitalLength * 2);
	glEnd();
	glColor3d(1, 1, 1);

	int minesTotald1 = b->totalMines / 10;
	int minesTotald2 = b->totalMines % 10;
	int minesRemainingd1 = b->displayedMinesRemaining / 10;
	int minesRemainingd2 = b->displayedMinesRemaining % 10;
	drawDigitalNumber(minesTotald1, 0.65, 0.5);
	drawDigitalNumber(minesTotald2, 0.7, 0.5);
	drawDigitalNumber(minesRemainingd1, 0.8, 0.5);
	drawDigitalNumber(minesRemainingd2, 0.85, 0.5);
	
}

void render()
{
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.2, 0.2, 0.2, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	drawBoard();
	drawButtons();
	drawGameState();
	drawStateBox();

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
		leftClick = true;
		b->state = 2;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		rightClick = true;
	}

	if (leftClick && rightClick)
	{
		leftClick = false;
		rightClick = false;
		if (b->withinBoard(trueX, trueY) && b->state > 0)
		{
			int r = (int)floor((b->top - trueY) / b->incrementY);
			int c = (int)floor((trueX - b->left) / b->incrementX);
			b->bothClick(r, c);
		}
	}
	else if (leftClick && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		leftClick = false;
		if (b->withinBoard(trueX, trueY) && b->state > 0)
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
	else if (rightClick)
	{
		rightClick = false;
		if (b->withinBoard(trueX, trueY) && b->state > 0)
		{
			int r = (int)floor((b->top - trueY) / b->incrementY);
			int c = (int)floor((trueX - b->left) / b->incrementX);
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
	glutInitWindowPosition(0, 0);
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
	buttons.push_back(Button(0.7, 0.9, 0.1, 0, newGameCallback));


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