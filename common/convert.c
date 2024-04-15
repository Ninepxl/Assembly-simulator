#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t string2uint_range(const char* str, int start, int end)
{
    if(end == -1)
    {
        end = strlen(str) - 1; 
    }
    int sign_bit = 0;
    int state = 0;
    uint64_t uv = 0;
    for (int i = start; i <= end; i++)
    {
        char c = str[i];
        if (state == 0)
        {
            if (c == '0')
            {
                state = 1;
                uv = 0;
                continue;
            }
            else if (c >= '1' && c <= '9')
            {
                state = 2;
                uv = c - '0';
                continue;
            }
            else if (c == '-')
            {
                state = 3;
                uv = 0;
                sign_bit = 1;
                continue;
            }
            else if (c == ' ')
            {
                continue;
            }
            else
            {
                goto flase_state;
            }
        }
        else if (state == 1)
        {
            if (c == 'x')
            {
                state = 4;
                continue;
            }
            else if (c >= '0' && c <= '9')
            {
                state = 2;
                uv = (c - '0') + uv * 10;
                continue;
            }
            else
            {
                goto flase_state;
            }
        }
        else if (state == 3)
        {
            if (c == '0')
            {
                state = 1;
                continue;
            }
            else if (c >= '0' && c <= '9')
            {
                state = 2;
                uv = uv * 10 + (c - '0');
                continue;
            }
            else
            {
                goto flase_state;
            }
        }
        else if (state == 2)
        {
            if (c >= '0' && c <= '9')
            {
                state = 2;
                uv = uv * 10 + (c - '0');
                continue;
            }
            else if (c == ' ')
            {
                state = 6;
                continue;
            }
            else
            {
                goto flase_state;
            }
        }
        else if (state == 4)
        {
            if ((c >= '0' && c <= '9'))
            {
                state = 5;
                uv = uv * 16 + (c - '0');
                continue;
            }
            else if (c >= 'a' && c <= 'f')
            {
                state = 5;
                uv = uv * 16 + (c - 'a') + 10;
                continue;
            }
            else
            {
                goto flase_state;
            }
        }
        else if (state == 5)
        {
            if ((c >= '0' && c <= '9'))
            {
                state = 5;
                uint64_t pv = uv;
                uv = uv * 16 + (c - '0');
                if (pv > uv)
                {
                    printf("overfollw uint64");
                    goto flase_state;
                }
                continue;
            }
            else if (c >= 'a' && c <= 'f')
            {
                state = 5;
                uv = uv * 16 + (c - 'a') + 10;
                uint64_t pv = uv;
                if (pv > uv)
                {
                    printf("overfollw uint64");
                    goto flase_state;
                }
                continue;
            }
            else
            {
                goto flase_state;
            }
        }
        else if (state == 6)
        {
            if (c == ' ')
            {
                state = 6;
                continue;
            }else
            {
                goto flase_state;
            }
        }
    }
    if (sign_bit == 0)
    {
        return uv;
    }
    // 当uv是有符号的时候,因为内存地址是二进制补码，要把 uv 转换成补码
    if (sign_bit == 1)
    {
        if ((uv & 0x8000000000000000) != 0)
        {
            printf("有符号的溢出");
            exit(0);
        }
        uint64_t sv = -1 * uv;
        return *(int64_t *)&sv;
    }
flase_state:
printf("Input error\n");
exit(0);
}