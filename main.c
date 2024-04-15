#include "headers/cpu.h"
#include "headers/memory.h"
#include <stdio.h>

static void TestOperation();
static void TestDramWR();

int main()
{
    TestOperation();
}

static void TestDramWR()
{
    uint64_t TestPaddress[4] =
    {
        0x12345,
        0xfadcf12,
        0x000120,
        0x000,
    };
    write64bits_dram(0x1, TestPaddress[0]);
    printf("%lx\n", read64bits_dram(0x1));
}

static inline void TestAssem()
{
    char Assembly_Inist[15][MAX_INSTRUCTION_CHAR] = 
    {
        "push   %rbp",             // 0
        "mov    %rsp,%rbp",        // 1
        "mov    %rdi,-0x18(%rbp)", // 2
        "mov    %rsi,-0x20(%rbp)", // 3
        "mov    -0x18(%rbp),%rdx", // 4
        "mov    -0x20(%rbp),%rax", // 5
        "add    %rdx,%rax",        // 6
        "mov    %rax,-0x8(%rbp)",  // 7
        "mov    -0x8(%rbp),%rax",  // 8
        "pop    %rbp",             // 9
        "retq",                    // 10
        "mov    %rdx,%rsi",        // 11
        "mov    %rax,%rdi",        // 12
        "callq  0",                // 13
        "mov    %rax,-0x8(%rbp)",  // 14
    };

    core_t *cr = &cores[0];
    cr->rip = (uint64_t)&Assembly_Inist[11];
    cr->reg.rax = 0xabcd;
    cr->reg.rbx = 0x8000670;
    cr->reg.rcx = 0x8000670;
    cr->reg.rdx = 0x12340000;
    cr->reg.rsi = 0x7ffffffee208;
    cr->reg.rdi = 0x1;
    cr->reg.rbp = 0x7ffffffee110;
    cr->reg.rsp = 0x7ffffffee0f0;
    cr->flags.flag_value = 0;

    write64bits_dram(vtop(0x7ffffffee110), 0x0000000000000000);
    write64bits_dram(vtop(0x7ffffffee108), 0x0000000000000000);
    write64bits_dram(vtop(0x7ffffffee100), 0x0000000000000000);
    write64bits_dram(vtop(0x7ffffffee0f8), 0x0000000000000000);
    write64bits_dram(vtop(0x7ffffffee0f0), 0x0000000000000000);
    printf_register(cr);

    int time = 0;
    while (time < 1)
    {
        instruction_cycle(cr);
        printf_register(cr);
        time++;
    }
    int match = 1;
    match = match && cr->reg.rax == 0x1234abcd;
    match = match && cr->reg.rbx == 0x8000670;
    match = match && cr->reg.rcx == 0x8000670;
    match = match && cr->reg.rdx == 0xabcd;
    match = match && cr->reg.rsi == 0x12340000;
    match = match && cr->reg.rdi == 0xabcd;
    match = match && cr->reg.rbp == 0x7ffffffee110;
    match = match && cr->reg.rsp == 0x7ffffffee0f0;

    if (match)
    {
        printf("register match\n");
    }
    else
    {
        printf("register mismatch\n");
    }

    match = match && (read64bits_dram(vtop(0x7ffffffee110)) == 0x0000000000000000); // rbp
    match = match && (read64bits_dram(vtop(0x7ffffffee108)) == 0x000000001234abcd);
    match = match && (read64bits_dram(vtop(0x7ffffffee100)) == 0x0000000012340000);
    match = match && (read64bits_dram(vtop(0x7ffffffee0f8)) == 0x000000000000abcd);
    match = match && (read64bits_dram(vtop(0x7ffffffee0f0)) == 0x0000000000000000); // rsp

    if (match)
    {
        printf("memory match\n");
    }
    else
    {
        printf("memory mismatch\n");
    }
}
