#ifndef __CPU__
#define __CPU__
#include <stdbool.h>
#include <stdint.h>

#include "debug.h"

#define MEM_PADDR_BITS 16 // 物理地址位数
#define MEM_SZ (1 << MEM_PADDR_BITS)
#define MAX_PERIPHERAL 16 // 外设的宽度

typedef enum
{
  ACCESS_WORD,
  ACCESS_HWORD,
  ACCESS_BYTE
} AccessMode;

/**
 * @brief 读数据
 * @param rel_addr 读的是什么地址
 * @param mode 什么访问模式
 * @return 返回读取到的数据
 */
typedef uint32_t (*PeripheralRCallback)(uint32_t rel_addr, AccessMode mode);

/**
 * @brief 写数据
 */
typedef void (*PeripheralWCallback)(uint32_t rel_addr, AccessMode mode, uint32_t data);

typedef struct
{
  const char *name;
  uint32_t base_addr;
  uint32_t len;
  PeripheralRCallback callback_r;
  PeripheralWCallback callback_w;
} peripheral_descr; // descript 外设的: 句柄

typedef struct
{
  uint32_t gpr[32]; // 寄存器堆
  uint32_t pc;
  uint32_t npc;
} riscv32_CPU_state;

typedef struct
{
  uint32_t inst;
  uint32_t pc;
} IF2ID;

/**
 * @brief 指令的 type
 *
 */
typedef enum
{
  R,
  I,
  S,
  B,
  U,
  J
} inst_format;

/**
 * @brief 立即数的形式
 *
 */
typedef enum
{
  IMM_I,
  IMM_S,
  IMM_B,
  IMM_U,
  IMM_J
} imm_format;

/**
 * @brief 内存带宽
 *
 */
typedef enum
{
  MEM_BYTE,
  MEM_HALF,
  MEM_WORD
} mem_width;

/**
 * @brief reg / mem / imm
 *
 */
typedef enum
{
  OP_TYPE_REG,
  OP_TYPE_MEM,
  OP_TYPE_IMM
} op_type;
typedef struct
{
  op_type type;
  uint32_t value;
} Operand;

/**
 * @brief debug
 *
 */
typedef struct
{
  __uint32_t wb_have_inst;
  __uint32_t wb_pc;
  __uint32_t wb_reg;
  __uint32_t wb_value;
  __uint32_t wb_ena;
} WB_info;

/**
 * @brief ALU 的 op
 *
 */
typedef enum
{
  OP_ADD,
  OP_SLT,
  OP_SLTU,
  OP_AND,
  OP_OR,
  OP_XOR,
  OP_SLL,
  OP_SRL,
  OP_SUB,
  OP_SRA,
  OP_INVALID,
  OP_ECALL
} alu_op_t;

typedef enum
{
  MEM_LB,
  MEM_LBU,
  MEM_LH,
  MEM_LHU,
  MEM_LW,
  MEM_SB,
  MEM_SH,
  MEM_SW
} mem_op_t;

typedef enum
{
  BR_EQ,
  BR_NEQ,
  BR_GE,
  BR_GEU,
  BR_LT,
  BR_LTU,
  //
  BR_JUMP,
  BR_JUMPREG
} br_op_t;

typedef enum
{
  WB_ALU,
  WB_PC,
  WB_LOAD
} wb_sel_t;

#include "riscv32_instdef.h"

typedef struct
{
  Decodeinfo_raw inst_raw_split;
  alu_op_t alu_op;
  mem_op_t mem_op;
  br_op_t br_op;
  wb_sel_t wb_sel; // TODO
  uint32_t next_pc;
  uint32_t is_jmp;
  uint32_t is_branch;
  uint32_t is_mem;
  Operand src1, src2;
  uint32_t store_val;
  uint32_t dst;
  uint32_t wb_en;
  uint32_t inst;
  uint32_t pc;
} ID2EX;

typedef struct
{
  uint32_t alu_out;
  uint32_t store_val;
  uint32_t is_mem;
  mem_op_t mem_op;
  wb_sel_t wb_sel;
  uint32_t wb_en;
  uint32_t dst;
  uint32_t branch_taken;
  uint32_t target_pc;
  uint32_t inst;
  uint32_t pc;
} EX2MEM;

typedef struct
{
  uint32_t alu_out;
  uint32_t load_out;
  wb_sel_t wb_sel;
  uint32_t wb_en;
  uint32_t dst;
  uint32_t branch_taken;
  uint32_t target_pc;
  uint32_t inst;
  uint32_t pc;
} MEM2WB;

#endif
