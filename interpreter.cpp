#include <iostream>
#include "parser/parser.hpp"
#include "windows.h"
#include <graphics.h>
#include "ui_main/GUI.hpp"

using namespace std;
using namespace module_parser;

int main(int argc, char** argv)
{
    SetConsoleOutputCP(65001);

    int const default_argc = 2;
    char* default_argv[] = {"interpreter.exe", "case.txt"};
    if(argc == 1)
    {
        argc = default_argc;
        argv = default_argv;
    }

    initLog();
    initWindow(800, 640);
    Parser p(argv[1]);
    p.run();
    getch();
    closegraph();

    return 0;
}
// g++ -g interpreter.cpp ./semantics/semantics.cpp ./lexer/lexer.cpp ./lexer/dfa.cpp ./lexer/token.cpp ./errorlog/*.cpp ./parser/parser.cpp ./ui_main/GUI.cpp -I ./include/ -lgraphics64 -luuid -lmsimg32 -lgdi32 -limm32 -lole32  -loleaut32 -lwinmm -lgdiplus -o interpreter.exe