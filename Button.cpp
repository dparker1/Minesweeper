#include "Button.h"

Button::Button(double l, double r, double t, double b, ButtonCallback func)
{
	left = l;
	right = r;
	top = t;
	bottom = b;
	this->func = func;
}

Button::~Button()
{
}

bool Button::withinButton(double x, double y)
{
	return x >= this->left &&
		x <= right &&
		y <= top &&
		y >= bottom;
}

void Button::click()
{
	func();
}
