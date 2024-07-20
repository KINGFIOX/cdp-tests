#include <stdio.h>

#include "cpu.h"

extern riscv32_CPU_state cpu;

extern uint32_t memory[];

IF2ID IF(uint32_t npc)
{
  IF2ID ret;
  Assert(npc < MEM_SZ, "PC out of boundary!\n");
  ret.inst = memory[npc >> 2]; // u32 寻址的
  ret.pc = npc;                // :NOTICE: 这里的时序是: 取指令完成以后, 才会 pc <- npc
  cpu.pc = npc;
  Log("\n=====");
  Log("Fetched instruction 0x%8.8x at PC=0x%8.8x", ret.inst, ret.pc);
  cpu.npc = cpu.pc + 4; // 当然我估计后面会改掉这个值(branch, jal, jalr), 默认是 npc = pc + 4
  return ret;
}
