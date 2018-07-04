#pragma once
class Cell
{
public:
	Cell();
	Cell(int mines);
	~Cell();

	bool mined();
	void setMines(int mines);

	int mines;
	bool flagged;
	bool revealed;
	bool blown;

};

