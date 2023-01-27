# Interpreter
编译原理大作业 绘图语言解释器

GUI的相关功能实现使用了第三方库 ege库

g++ -g interpreter.cpp ./semantics/semantics.cpp ./lexer/lexer.cpp ./lexer/dfa.cpp ./lexer/token.cpp ./errorlog/*.cpp ./parser/parser.cpp ./ui_main/GUI.cpp -I ./include/ -lgraphics64 -luuid -lmsimg32 -lgdi32 -limm32 -lole32  -loleaut32 -lwinmm -lgdiplus -o interpreter.exe