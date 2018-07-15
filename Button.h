#pragma once

typedef void(*ButtonCallback)(void);

class Button
{
public:
	Button(double left, double right, double top, double bottom, ButtonCallback func);
	~Button();
	bool withinButton(double x, double y);
	void click();


	double left;
	double right;
	double top;
	double bottom;
	ButtonCallback func;
};

