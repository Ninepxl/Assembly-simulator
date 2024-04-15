#include<stdio.h>

void Dug_printf(int DEBUG_INF,const char* str)
{
    if(DEBUG_INF == 1)
    {
        printf("ISA ERROR %s",str);
    }
}