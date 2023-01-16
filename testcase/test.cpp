#include <iostream>
#include <cmath>
#include <windows.h>
#include <graphics.h>
#include "ui_main/GUI.hpp"

using namespace std;

typedef unsigned int uint;

int _WIDTH;
int _HEIGHT;

void draw(uint x, uint y)
{
    color_t color = EGERGB(255, 0, 0);
    putpixel(x, y, color);
    putpixel(x+1, y, color);
    putpixel(x, y+1, color);
    putpixel(x+1, y+1, color);
}

void showMessage(string s)
{
    cleardevice();
    outtextrect(_WIDTH/4, _HEIGHT/2 - 20 , _WIDTH/2 , 20, s.c_str());
    getch();
}

void initWindow(int width, int height)
{
    _WIDTH = width;
    _HEIGHT = height;
    initgraph(width, height, 0);
    setbkcolor(EGERGB(0xFF, 0xFF, 0xFF));
    setfont(-20, 0, "黑体");
}
int main()
{
    initWindow();

    for(int i = 100; i <= 900; i++)
    {
        draw(i, 400);
    }

    showMessage("fudadsadadadasdaadck");
    //等待用户按键
    getch();
    //关闭图形界面
    closegraph();
}