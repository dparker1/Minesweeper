#pragma once
#include "Cell.h"
class Board
{
public:
	Board(int width, int height, int mines);
	~Board();

	Cell* getCell(int row, int column);
	bool isMined(int row, int column);
	bool isRevealed(int row, int column);
	bool isFlagged(int row, int column);
	void reveal(int row, int column);
	void changeFlag(int row, int column);
	void click(int row, int column);
	int mines(int row, int column);

	int width;
	int height;
	int totalMines;
	Cell** board;
};
