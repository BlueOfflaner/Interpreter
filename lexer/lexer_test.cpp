#include "lexer/lexer.hpp"
#include <iostream>
#include <cstdio>
#include <windows.h>
using namespace std;
using namespace module_lexer;

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(65001);
    argv[1] = "C:\\Users\\26806\\Desktop\\class\\Compilers\\Interpreter\\testcase\\lexTest.txt";
    Lexer lexer;

    if(0 == lexer.initScanner(argv[1]))
    {
        cout << "Open Source File Error!\n";
        return 2;
    }
    printf("记号类别    字符串      常数值      函数指针\n");
    printf("____________________________________________\n");
    Token token;
    while(1) 
    { 
        token = lexer.getToken();		// 通过词法分析器获得一个记号
        if(token.getType() != NONTOKEN)	// 打印记号的内容
        cout << token.getType() << "  " << token.getText() << "   " << token.getValue() << "   " << token.getFuncPtr() << endl;
        else    break;			// 源程序结束，退出循环
    };
    printf("____________________________________________\n");
    getchar();
}