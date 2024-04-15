#include "headers/memory.h"
#include <stdint.h>
#include "headers/cpu.h"
#include "headers/common.h"
#include <stdio.h>

uint64_t read64bits_dram(uint64_t paddr)
{
    uint64_t value = 0x0;

    if (DEBUG_ENABLE_SRAM_CACHE == 1)
    {
        return 0;
    }

    // 做为最高的8位
    value += (((uint64_t)physical_mem[paddr + 0]) << 0);  // ff
    value += (((uint64_t)physical_mem[paddr + 1]) << 8);  // ff00
    value += (((uint64_t)physical_mem[paddr + 2]) << 16); // ff0000
    value += (((uint64_t)physical_mem[paddr + 3]) << 24);
    value += (((uint64_t)physical_mem[paddr + 4]) << 32);
    value += (((uint64_t)physical_mem[paddr + 5]) << 40);
    value += (((uint64_t)physical_mem[paddr + 6]) << 48);
    value += (((uint64_t)physical_mem[paddr + 7]) << 56);
    return value;
}

void write64bits_dram(uint64_t paddr, uint64_t data)
{
    if (DEBUG_ENABLE_SRAM_CACHE == 1)
    {
        return;
    }

    physical_mem[paddr + 0] = (uint8_t)((data >> 0) & 0xff);
    physical_mem[paddr + 1] = (uint8_t)((data >> 8) & 0xff);
    physical_mem[paddr + 2] = (uint8_t)((data >> 16) & 0xff);
    physical_mem[paddr + 3] = (uint8_t)((data >> 24) & 0xff);
    physical_mem[paddr + 4] = (uint8_t)((data >> 32) & 0xff);
    physical_mem[paddr + 5] = (uint8_t)((data >> 40) & 0xff);
    physical_mem[paddr + 6] = (uint8_t)((data >> 48) & 0xff);
    physical_mem[paddr + 7] = (uint8_t)((data >> 54) & 0xff);
}