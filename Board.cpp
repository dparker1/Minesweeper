#include "Board.h"
#include <cstdlib>
#include <ctime>

Board::Board(int width, int height, int mines)
{
	this->width = width;
	this->height = height;
	totalMines = mines;
	displayedMinesRemaining = mines;
	actualMinesRemaining = mines;
	int size = width * height;
	board = new Cell*[size];
	state = 1;
	updateBoardIncrements();

	int m = 0;
	srand(time(NULL));
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			board[j * width + i] = new Cell(0);
		}
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int r = rand() % totalMines;
			if (r == 0)
			{
				if (!isMined(j, i))
				{
					board[j * width + i]->mines = -1;
					m++;
				}
			}
			if (m == totalMines)
			{
				break;
			}
		}
		if (m == totalMines)
		{
			break;
		}
		if (i == width - 1 && m < totalMines)
		{
			i = -1;
		}
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Cell* c = getCell(j, i);
			if (!c->mined())
			{
				c->setMines(getMinesSurrounding(j, i));
			}
		}
	}
}

Board::~Board()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			delete &board[j * width + i];
		}
	}
	delete board;
}

Cell* Board::getCell(int row, int column)
{
	return board[row * width + column];
}

bool Board::isMined(int row, int column)
{
	return getCell(row, column)->mined();
}

bool Board::isRevealed(int row, int column)
{
	return getCell(row, column)->revealed;
}

bool Board::isFlagged(int row, int column)
{
	return getCell(row, column)->flagged;
}

bool Board::withinBoard(double x, double y)
{
	return x >= left &&
		x <= right &&
		y <= top &&
		y >= bottom;
}

void Board::reveal(int row, int column)
{
	if (!isRevealed(row, column))
	{
		getCell(row, column)->revealed = true;
		getCell(row, column)->flagged = false;
		if (mines(row, column) == 0)
		{
			for (int r = row - 1; r <= row + 1; r++)
			{
				for (int c = column - 1; c <= column + 1; c++)
				{
					if (!(r == row && c == column) && (r >= 0 && r < height && c >= 0 && c < width) && !isRevealed(r,c)) {
						reveal(r, c);
					}
				}
			}
		}
	}
}

void Board::changeFlag(int row, int column)
{
	getCell(row, column)->flagged = !isRevealed(row, column) && !isFlagged(row, column);
	calculateMinesRemaining();
}

void Board::click(int row, int column)
{
	reveal(row, column);
	state = (isMined(row, column) ? -1 : 1);
}

void Board::bothClick(int row, int column)
{
	if (isRevealed(row, column))
	{
		int mines = getMinesSurrounding(row, column);
		for (int r = row - 1; r <= row + 1; r++)
		{
			for (int c = column - 1; c <= column + 1; c++)
			{
				if (r >= 0 && r < height && c >= 0 && c < width && isMined(r, c) && isFlagged(r, c))
				{
					mines--;
				}
			}
		}
		if (mines == 0)
		{
			for (int r = row - 1; r <= row + 1; r++)
			{
				for (int c = column - 1; c <= column + 1; c++)
				{
					if (r >= 0 && r < height && c >= 0 && c < width && !isMined(r, c))
					{
						reveal(r, c);
					}
				}
			}
		}
	}
	state = 1;
}

void Board::calculateMinesRemaining()
{
	int flags = 0;
	actualMinesRemaining = totalMines;
	for (int c = 0; c < width; c++)
	{
		for (int r = 0; r < height; r++)
		{
			Cell* curr = getCell(r, c);
			if (isFlagged(r, c))
			{
				flags++;
				if (isMined(r, c))
				{
					actualMinesRemaining--;
				}
			}
		}
	}
	displayedMinesRemaining = totalMines - flags;
	if (actualMinesRemaining == 0)
	{
		state = 0;
	}
}

void Board::updateBoardIncrements()
{
	incrementX = (right - left) / (width);
	incrementY = (top - bottom) / (height);
}

int Board::getMinesSurrounding(int row, int column)
{
	int mines = 0;
	for (int r = row - 1; r <= row + 1; r++)
	{
		for (int c = column - 1; c <= column + 1; c++)
		{
			if (r >= 0 && r < height && c >= 0 && c < width)
			{
				mines += isMined(r, c);
			}
		}
	}
	return mines;
}

int Board::mines(int row, int column)
{
	return getCell(row, column)->mines;
}