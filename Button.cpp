#include "Button.h"



Button::Button(double w, double h, double x, double y, ButtonCallback func)
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

bool Button::withinButton(double x, double y)
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
