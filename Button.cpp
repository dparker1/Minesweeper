#include "Button.h"



Button::Button(int w, int h, int x, int y, ButtonCallback func)
{
	width = w;
	height = h;
	this->x = x;
	this->y = y;
	this->func = func;
}

Button::~Button()
{
}

bool Button::withinButton(int x, int y)
{
	return x >= this->x &&
		x <= (this->x + width) &&
		y <= (this->y + height) &&
		y >= this->y;
}

void Button::click()
{
	func();
}
