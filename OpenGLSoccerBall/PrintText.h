#pragma once


#include <glut.h>


enum FONT_COLOR
{
	BLACK,
	WHITE,
	RED,
	GREEN, 
	BLUE
};


void makeRasterFont(void);
void drawString(const char* str, int x, int y, float color[4]);
void drawString(const char* str, int x, int y, int color);

