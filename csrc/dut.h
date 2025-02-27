#ifndef __DUT__
#define __DUT__

#include <sys/types.h>
#include <verilated_vcd_c.h>

#include "cpu.h"

// template <class MODULE>
class TESTBENCH
{
  using MODULE = VminiRV_SoC;

public:
  VerilatedVcdC *vltdump; // dump 文件句柄
  MODULE *dut;
  unsigned long count;

  TESTBENCH()
  {
    Verilated::traceEverOn(true);
    dut = new MODULE;
    count = 0;
  }

  // Open/create a trace file
  void opentrace(const char *vcdName)
  {
    if (!vltdump)
    {
      vltdump = new VerilatedVcdC;
      dut->trace(vltdump, 99);
      vltdump->open(vcdName);
    }
  }

  ~TESTBENCH()
  {
    close_trace();
    if (!vltdump)
      delete vltdump;
    delete dut;
  }

  // Close a trace file
  void close_trace(void)
  {
    if (vltdump)
    {
      vltdump->close();
      vltdump = NULL;
    }
  }

  WB_info tick(void)
  {
    count++;

    dut->clock = 0;
    dut->eval();

    if (vltdump)
      vltdump->dump((vluint64_t)(10 * count - 1));

    // Repeat for the positive edge of the clock
    dut->clock = 1;
    dut->eval();
    if (vltdump)
      vltdump->dump((vluint64_t)(10 * count));

    // Now the negative edge
    dut->clock = 0;
    dut->eval();
    if (vltdump)
    {
      vltdump->dump((vluint64_t)(10 * count + 5));
      vltdump->flush();
    }
    WB_info ret;
    ret.wb_have_inst = dut->io_dbg_wb_have_inst;
    ret.wb_pc = dut->io_dbg_wb_pc;
    ret.wb_ena = dut->io_dbg_wb_ena;
    ret.wb_reg = dut->io_dbg_wb_reg;
    ret.wb_value = dut->io_dbg_wb_value;
    ret.inst_valid = dut->io_dbg_inst_valid;
    return ret;
  }
};
#endif
