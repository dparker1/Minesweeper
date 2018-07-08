#include "Board.h"
#include <cstdlib>
#include <ctime>

Board::Board(int width, int height, int mines)
{
	this->width = width;
	this->height = height;
	totalMines = mines;
	int size = width * height;
	board = new Cell*[size];
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
				int mines = 0;
				for (int r = j - 1; r <= j + 1; r++)
				{
					for (int c = i - 1; c <= i + 1; c++)
					{
						if (r >= 0 && r < height && c >= 0 && c < width)
						{
							mines += isMined(r, c);
						}
					}
				}
				c->setMines(mines);
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
					if (!(r == row && c == column) && (r >= 0 && r < height && c >= 0 && c < width)) {
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
}

void Board::click(int row, int column)
{
	reveal(row, column);
}

int Board::mines(int row, int column)
{
	return getCell(row, column)->mines;
}