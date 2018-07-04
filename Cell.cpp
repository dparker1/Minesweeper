#include "Cell.h"

Cell::Cell()
{
	Cell(0);
}

Cell::Cell(int mines)
{
	this->mines = mines;
	flagged = false;
	blown = false;
	revealed = false;
}

Cell::~Cell()
{
}

bool Cell::mined()
{
	return mines == -1;
}

void Cell::setMines(int mines)
{
	this->mines = mines;
}