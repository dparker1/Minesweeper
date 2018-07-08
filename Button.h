#pragma once

typedef void(*ButtonCallback)(void);

class Button
{
public:
	Button(int width, int height, int x, int y, ButtonCallback func);
	~Button();
	bool withinButton(int x, int y);
	void click();


	int width;
	int height;
	int x;
	int y;
	ButtonCallback func;
};

