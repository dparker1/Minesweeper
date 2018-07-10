#pragma once

typedef void(*ButtonCallback)(void);

class Button
{
public:
	Button(double width, double height, double x, double y, ButtonCallback func);
	~Button();
	bool withinButton(double x, double y);
	void click();


	double width;
	double height;
	double x;
	double y;
	ButtonCallback func;
};

