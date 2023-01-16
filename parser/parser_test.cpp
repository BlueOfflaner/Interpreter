#include <iostream>
#include "parser/parser.hpp"
#include "windows.h"
using namespace std;
using namespace module_parser;

int main()
{
    SetConsoleOutputCP(65001);
    initLog();
    //initWindow();
    Parser p("C:\\Users\\26806\\Desktop\\class\\Compilers\\Interpreter\\testcase\\lexTest.txt");
    p.run();
    //getch();
    //closegraph();
    getchar();
}