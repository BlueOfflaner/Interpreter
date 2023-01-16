#include <iostream>
#include <graphics.h>
#include "ui_main/GUI.hpp"

using namespace std;

void draw(uint x, uint y)
{
    color_t color = EGERGB(255, 0, 0);
    putpixel(x, y, color);
    putpixel(x+1, y, color);
    putpixel(x, y+1, color);
    putpixel(x+1, y+1, color);
}

void initWindow(int width, int height)
{
    initgraph(width, height, 0);
    setbkcolor(EGERGB(0xFF, 0xFF, 0xFF));
}