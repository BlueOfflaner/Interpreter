#ifndef _LOGTREE_HPP
#define _LOGTREE_HPP

//#define _LOG_PATH ""
#define _LOG_PATH "logTree.txt"

#include <string>
#include <stdlib.h>
#include <stdarg.h>

void initLog();

void logTree(const char* format, ...);

void closeLog();

#endif