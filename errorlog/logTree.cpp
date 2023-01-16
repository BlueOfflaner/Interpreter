#include "errorlog/logTree.hpp"


static FILE* fpLog = NULL;

void initLog()
{
    fpLog = fopen(_LOG_PATH, "w");
}

void logTree(const char* format, ...)
{
    va_list args;
    //if(fpLog == NULL) exit(0);
    FILE* fp = (fpLog == NULL) ? stdout : fpLog;

    va_start(args, format);
    vfprintf(fp, format, args);
    va_end(args);
}

void closeLog()
{
    fclose(fpLog);
}