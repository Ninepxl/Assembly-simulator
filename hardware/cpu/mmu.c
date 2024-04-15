#include <stdint.h>
#include "headers/cpu.h"
#include "headers/memory.h"

// 将虚拟内存地址转换到物理内存的地址上这里只是单纯是取模
uint64_t vtop(uint64_t viraddress)
{
    return viraddress % PHYSICAL_NENORY_SPACE;
}



