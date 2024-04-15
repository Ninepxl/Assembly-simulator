#ifndef MEM_H
#define MEM_H

#include <stdint.h>

#define PHYSICAL_NENORY_SPACE 65536

uint8_t physical_mem[PHYSICAL_NENORY_SPACE];

uint64_t read64bits_dram(uint64_t paddr);

void write64bits_dram(uint64_t paddr,uint64_t data);

void readinst_dram(uint64_t paddr, char *buf);

void writeinst_dram(uint64_t paddr, char* str);

#endif