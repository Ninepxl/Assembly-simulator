#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#define NUM_CORES 1
#define MAX_INSTRUCTION_CHAR 64 // 字符指令的长度
#define NUM_INSTRTYPE 14 // 
typedef struct REGISTER_STRUCT
{
    union
    {
        uint64_t rax;
        uint32_t eax;
        uint16_t ax;
        struct
        {
            uint8_t al;
            uint8_t ah;
        };
    };
    union
    {
        uint64_t rbx;
        uint32_t ebx;
        uint16_t bx;
        struct
        {
            uint8_t bl;
            uint8_t bh;
        };
    };
    union
    {
        uint64_t rcx;
        uint32_t ecx;
        uint16_t cx;
        struct
        {
            uint8_t cl;
            uint8_t ch;
        };
    };
    union
    {
        uint64_t rdx;
        uint32_t edx;
        uint16_t dx;
        struct
        {
            uint8_t dl;
            uint8_t dh;
        };
    };
    union
    {
        uint64_t rsi;
        uint32_t esi;
        uint16_t si;
        struct
        {
            uint8_t sil;
            uint8_t sih;
        };
    };
    union
    {
        uint64_t rdi;
        uint32_t edi;
        uint16_t di;
        struct
        {
            uint8_t dil;
            uint8_t dih;
        };
    };
    // the rbp is point to the stack bottom
    union
    {
        uint64_t rbp;
        uint32_t ebp;
        uint16_t bp;
        struct
        {
            uint8_t bpl;
            uint8_t bph;
        };
    };
    // the rsp is point to stack top
    union
    {
        uint64_t rsp;
        uint32_t esp;
        uint16_t sp;
        struct
        {
            uint8_t spl;
            uint8_t sph;
        };
    };
    union
    {
        uint64_t r8;
        uint32_t r8d;
        uint16_t r8w;
    };
    union
    {
        uint64_t r9;
        uint32_t r9d;
        uint16_t r9w;
    };
    union
    {
        uint64_t r10;
        uint32_t r10d;
        uint16_t r10w;
    };
    union
    {
        uint64_t r11;
        uint32_t r11d;
        uint16_t r11w;
    };
    union
    {
        uint64_t r12;
        uint32_t r12d;
        uint16_t r12w;
    };
    union
    {
        uint64_t r13;
        uint32_t r13d;
        uint16_t r13w;
    };
    union
    {
        uint64_t r14;
        uint32_t r14d;
        uint16_t r14w;
    };
    union
    {
        uint64_t r15;
        uint32_t r15d;
        uint16_t r15w;
    };

} reg_t;

typedef struct CPU_FLAGS_STRUCT
{
    union
    {
        uint64_t flag_value;
        struct
        {
            // check unsigned overflow
            uint16_t CF;
            // check result whether is 0
            uint16_t ZF;
            // check highest bit whether is 1
            uint16_t SF;
            // 检测有符号的数是否溢出
            uint16_t OF;
        };
    };
} cpu_flag_t;

typedef struct CORE_STRUCT
{
    // the rip is point to the instruct of running programe
    struct
    {
        uint64_t rip;
        uint32_t eip;
    };
    reg_t reg;
    cpu_flag_t flags;
} core_t;

core_t cores[NUM_CORES];
uint64_t vtop(uint64_t viraddress);
void instruction_cycle(core_t *cr);
void printf_register(core_t *cr);
#endif