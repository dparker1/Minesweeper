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
	bool withinBoard(int x, int y);
	void reveal(int row, int column);
	void changeFlag(int row, int column);
	void click(int row, int column);
	void calculateMinesRemaining();
	int mines(int row, int column);

	int width;
	int height;
	double incrementX;
	double incrementY;
	double top = 0.95,
		bottom = -0.95,
		left = -0.95,
		right = 0.6;
	int totalMines;
	int displayedMinesRemaining;
	int actualMinesRemaining;
	Cell** board;

private:
	void updateBoardIncrements();
};

