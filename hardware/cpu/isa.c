#include <stdint.h>
#include "headers/cpu.h"
#include <stdio.h>
#include "headers/memory.h"
#include <string.h>
#include <stdlib.h>
/*
    CPU 指令集
*/

/*=======================================*/
/*              指令结构                   */
/*=======================================*/
typedef enum OPERATION_TYPE
{
    INST_MOV,   // 0
    INST_PUSH,  // 1
    INST_POP,   // 2
    INST_LEAVE, // 3
    INST_CALL,  // 4
    INST_RET,   // 5
    INST_ADD,   // 6
    INST_SUB,   // 7
    INST_CMP,   // 8
    INST_JNE,   // 9
    INST_JMP,   // 10
} op_t;

typedef enum OPERATION_DATA_TYPE
{
    IMM,                  // 0
    REG,                  // 1
    MEM_IMM,              // 2
    MEM_REG,              // 3
    MEM_IMM_REG,          // 4
    MEM_REG_REG,          // 5
    MEM_IMM_REG_REG,      // 6
    MEM_REG_SCLE,         // 7
    MEM_IMM_REG_SCLE,     // 8
    MEM_REG_REG_SCLE,     // 9
    MEM_IMM_REG_REG_SCLE, // 10
    EMPTY
} od_type_t;

typedef struct OPERATION_DATA
{
    int64_t imm;   // 立即数
    uint64_t reg1; // 寄存器的地址
    uint64_t reg2;
    uint64_t sacle; // imm(reg,reg,sacle)
    od_type_t type;
} od_t;

typedef struct INSTRUCT_TYPE
{
    op_t op;
    od_t src;
    od_t dst;
} inst_t;

const char *reg_name[72] =
    {
        "%rax",
};

void parse_instruction(const char *str, inst_t *inst, core_t *cr); // 字符串指令解析
void parse_operand(const char *str, od_t *od, core_t *cr);         // 字符串操作数解析
uint64_t string2uint_range(const char *str, int start, int end);
uint64_t decode_operand(od_t *od);                           // 译码根据od_type_t 如果是访存就返回内存的地址
typedef void (*handler_t)(od_t *src, od_t *dst, core_t *cr); // 函数指针数组
static void mov_handler(od_t *src_od, od_t *dst_od, core_t *cr);
uint64_t get_reg_addr(const char *str, core_t *cr);
/*
    解析操作数，如果是立即数，就直接返回，如果是寄存器，就返回寄存器的地址，如果是内存，就返回内存的地址
*/
uint64_t decode_operand(od_t *od)
{
    if (od->type == IMM)
    {
        return od->imm;
    }
    else if (od->type == REG)
    {
        return od->reg1;
    }
    else
    {
        uint64_t viraddr = 0;
        if (od->type == MEM_IMM)
        {
            viraddr = od->imm;
        }
        else if (od->type == MEM_REG)
        {
            viraddr = *(uint64_t *)od->reg1;
        }
        else if (od->type == MEM_IMM_REG)
        {
            viraddr = od->imm + (*(uint64_t *)od->reg1);
        }
        else if (od->type == MEM_REG_REG)
        {
            viraddr = (*(uint64_t *)od->reg1) + (*(uint64_t *)od->reg2);
        }
        else if (od->type == MEM_IMM_REG_REG)
        {
            viraddr = (*(uint64_t *)od->reg1) + (*(uint64_t *)od->reg2) + od->imm;
        }
        else if (od->type == MEM_REG_SCLE)
        {
            viraddr = (*(uint64_t *)od->reg1) * od->sacle;
        }
        else if (od->type == MEM_IMM_REG_SCLE)
        {
            viraddr = ((*(uint64_t *)od->reg1) + od->imm) * od->sacle;
        }
        else if (od->type == MEM_REG_REG_SCLE)
        {
            viraddr = ((*(uint64_t *)od->reg1) + (*(uint64_t *)od->reg2)) * od->sacle;
        }
        else if (od->type == MEM_IMM_REG_REG_SCLE)
        {
            viraddr = ((*(uint64_t *)od->reg1) + (*(uint64_t *)od->reg1) + od->imm) * od->sacle;
        }
        return viraddr;
    }
}

uint64_t get_reg_addr(const char *str, core_t *cr)
{
    if (strcmp(str, "%rax") == 0)
    {
        return (uint64_t) & (cr->reg.rax);
    }
    else if (strcmp(str, "%rbx") == 0)
    {
        return (uint64_t) & (cr->reg.rbx);
    }
    else if (strcmp(str, "%rcx") == 0)
    {
        return (uint64_t) & (cr->reg.rcx);
    }
    else if (strcmp(str, "%rdx") == 0)
    {
        return (uint64_t) & (cr->reg.rdx);
    }
    else if (strcmp(str, "%rsi") == 0)
    {
        return (uint64_t) & (cr->reg.rsi);
    }
    else if (strcmp(str, "%rdi") == 0)
    {
        return (uint64_t) & (cr->reg.rdi);
    }
    else if (strcmp(str, "%rbp") == 0)
    {
        return (uint64_t) & (cr->reg.rbp);
    }
    else if (strcmp(str, "%rsp") == 0)
    {
        return (uint64_t) & (cr->reg.rsp);
    }
    else if (strcmp(str, "%r8") == 0)
    {
        return (uint64_t) & (cr->reg.r8);
    }
    else if (strcmp(str, "%r9") == 0)
    {
        return (uint64_t) & (cr->reg.r9);
    }
    else if (strcmp(str, "%r10") == 0)
    {
        return (uint64_t) & (cr->reg.r10);
    }
    else if (strcmp(str, "%r11") == 0)
    {
        return (uint64_t) & (cr->reg.r11);
    }
    else if (strcmp(str, "%r12") == 0)
    {
        return (uint64_t) & (cr->reg.r12);
    }
    else if (strcmp(str, "%r13") == 0)
    {
        return (uint64_t) & (cr->reg.r13);
    }
    else if (strcmp(str, "%r14") == 0)
    {
        return (uint64_t) & (cr->reg.r14);
    }
    else if (strcmp(str, "%r15") == 0)
    {
        return (uint64_t) & (cr->reg.r15);
    }
    else
    {
        printf("parse register %s error\n", str);
        exit(0);
    }
}

static handler_t handler_table[NUM_INSTRTYPE] =
    {
        &mov_handler, // 0
};

void instruction_cycle(core_t *cr)
{
    const char *str = (const char *)cr->rip; // 拿到指令字符串
    inst_t inst;
    parse_instruction(str, &inst, cr);          // 解析指令字符串
    handler_t handler = handler_table[inst.op]; // 下标对应操作码
    handler(&(inst.src), &(inst.dst), cr);      // 执行指令
}

void printf_register(core_t *cr)
{
    printf("rax => %lx\n", cr->reg.rax);
    printf("rbx => %lx\n", cr->reg.rbx);
    printf("rcx => %lx\n", cr->reg.rcx);
    printf("rdx => %lx\n", cr->reg.rdx);
    printf("rsi => %lx\n", cr->reg.rsi);
    printf("rdi => %lx\n", cr->reg.rdi);
    printf("rbp => %lx\n", cr->reg.rbp);
    printf("rsp => %lx\n", cr->reg.rsp);
}

void printf_memory(core_t *cr)
{
}

void next_rip(core_t *cr)
{
    cr->rip += sizeof(char) * MAX_INSTRUCTION_CHAR;
}

void rereset_cflags(core_t *cr)
{
    cr->flags.flag_value = 0;
}

static void mov_handler(od_t *src_od, od_t *dst_od, core_t *cr)
{
    uint64_t src = decode_operand(src_od);
    uint64_t dst = decode_operand(dst_od);
    if (src_od->type == REG && dst_od->type == REG)
    {
        *(uint64_t *)dst = *(uint64_t *)src;
        next_rip(cr);
        rereset_cflags(cr);
    }
    else if (src_od->type == REG && dst_od->type == MEM_IMM)
    {
        // 将内存中的值写入寄存器
        // dst = src
        // 将寄存器中的值写入内存
        write64bits_dram(vtop(dst), *(uint64_t *)src);
        next_rip(cr);
        rereset_cflags(cr);
    }
    else if (src_od->type == MEM_IMM && dst_od->type == REG)
    {
        // src : memory address
        // dst : register
        // dst = src
        uint64_t value = read64bits_dram(vtop(src));
        *(uint64_t *)dst = value;
        next_rip(cr);
        rereset_cflags(cr);
    }
    else if (src_od->type == REG && dst_od->type == MEM_IMM_REG)
    {
        // src : reg
        // dst : mem
        // dst = src
        write64bits_dram(vtop(dst), *(uint64_t *)src);
        next_rip(cr);
        rereset_cflags(cr);
    }
    else if (src_od->type == MEM_IMM_REG && dst_od->type == REG)
    {
        uint64_t value = read64bits_dram(vtop(src));
        *(uint64_t *)dst = value;
        next_rip(cr);
        rereset_cflags(cr);
    }
}

static void add_handler(od_t *src, od_t *dst, core_t *cr)
{
    *(uint64_t *)dst = *(uint64_t *)dst + *(uint64_t *)src;
    next_rip(cr);
    // TODO : 标志位改变
}

static void push_handler(od_t *src, od_t *dst, core_t *cr)
{
}

static void pop_handler(od_t *src, od_t *dst, core_t *cr)
{
}

void parse_instruction(const char *str, inst_t *inst, core_t *cr)
{
    char src_str[64] = {'\0'};
    int src_len = 0;
    char dst_str[64] = {'\0'};
    int dst_len = 0;
    char op_str[64] = {'\0'};
    int op_len = 0;
    char c;
}

void parse_operand(const char *str, od_t *od, core_t *cr)
{
    od->imm = 0;
    od->reg2 = 0;
    od->reg1 = 0;
    od->sacle = 0;
    od->type = EMPTY;
    uint64_t length = strlen(str);
    if(length == 0)
    {
        return;
    }
    if (str[0] == '$') // 是立即数，就把这个字符串变成数字
    {
        od->type = IMM;
        od->imm = string2uint_range(str, 1, -1);
    }
    else if (str[0] == '%')
    {
        od->type = REG;
        od->reg1 = get_reg_addr(str, cr);
    }
    else
    {
        char imm_str[64] = {'\0'};
        char reg1_str[64] = {'\0'};
        char reg2_str[64] = {'\0'};
        char scale_str[64] = {'\0'};
        uint64_t length_imm = 0;
        uint64_t length_reg1 = 0;
        uint64_t length_reg2 = 0;
        uint64_t length_scale = 0;
        int bracket = 0; // 括号
        int comma = 0;   // 逗号
        for (int i = 0; i < length; i++)
        {
            char c = str[i];
            if (c == '(' || c == ')')
            {
                bracket++;
                continue;
            }
            else if (c == ',')
            {
                comma++;
                continue;
            }
            else if(c == ' ')
            {
                continue;
            }
            else 
            {
                if(bracket == 0) // imm
                {
                    imm_str[length_imm] = c;
                    length_imm++;
                }
                else if(bracket == 1) 
                {
                    if(comma == 0) // ...(
                    {
                        reg1_str[length_reg1] = c;
                        length_reg1++;
                    }
                    else if(comma == 1) // ...(...,
                    {
                        reg2_str[length_reg2] = c;
                        length_reg2++;
                    }
                    else if(comma == 2) // ...(...,...,)
                    {
                        scale_str[length_scale] = c;
                        length_scale++;
                    }
                }
            }
        }
        //TODO: od->type 的确定 和 od的赋值
        if(length_imm > 0)
        {
            od->imm = string2uint_range(imm_str,0,-1);
            if(bracket == 0)
            {
                od->type = 2;
                return;
            }
        }

        // set value
        if(length_reg1 > 0)
        {
            od->reg1 = get_reg_addr(reg1_str,cr);
        }
        if(length_reg2 > 0)
        {
            od->reg2 = get_reg_addr(reg2_str,cr);
        }
        if(length_scale > 0)
        {
            od->sacle = string2uint_range(scale_str,0,-1);
            if(od->sacle != 1 && od->sacle != 2 && od->sacle != 4 && od->sacle != 8)
            {
                printf("scale error");
            }
        }

        // set od->type
        if(comma == 0) // ()
        {
            od->type = MEM_REG;
        }
        else if(comma == 1) 
        {
            if(length_reg1 == 0)    // (,reg2)
            {
                od->type = MEM_REG;
            }
            else                     // (reg1,reg2)
            {
                od->type = MEM_REG_REG;
            }
        }
        else if(comma == 2)
        {
            od->type = MEM_REG_REG_SCLE;
        }

        if(length_imm > 0 && bracket != 0)
        {
            od->type++;
        }
    }
}

void TestOperation()
{
    const char* od_str[5] = 
    {
        "$123",
        "12",
        "%rax",
        "(%rax)",
        "12(%rax,,1)"
    };
    core_t *cr = &cores[0];
    cr->reg.rax = 0xabcd;
    cr->reg.rbx = 0x8000670;
    cr->reg.rcx = 0x8000670;
    cr->reg.rdx = 0x12340000;
    cr->reg.rsi = 0x7ffffffee208;
    cr->reg.rdi = 0x1;
    cr->reg.rbp = 0x7ffffffee110;
    cr->reg.rsp = 0x7ffffffee0f0;

    for (int i = 0; i < 5; i++)
    {
        od_t od_;
        parse_operand(od_str[i],&od_,cr);
        printf("%lx\n",od_.sacle);
      //  printf("%s => imm : %lx reg1 : %lx reg2 : %lx sacle : %lx type : %d \n",od_str[i],od_.imm,*(uint64_t*)od_.reg1,*(uint64_t*)od_.reg2,od_.sacle,od_.type);
    }
}