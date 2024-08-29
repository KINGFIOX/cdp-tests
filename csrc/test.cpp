#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VminiRV_SoC.h"
#include "dut.h"
#include "verilated.h"

/* ---------- ---------- 前向声明 ---------- ---------- */

extern void init_cpu();
extern WB_info cpu_run_once();

/* ---------- ---------- 全局变量 ---------- ---------- */

// TESTBENCH<VminiRV_SoC> *top;
TESTBENCH* top;
VminiRV_SoC* top_module;
double main_time = 0;
double sc_time_stamp() { return main_time; }

/* ---------- ----------  ---------- ---------- */

void reset_all()
{
    printf("[mycpu] Resetting ...\n");
    top_module->reset = 1;
    for (int i = 0; i < 20; i++) {
        top->tick();
    }
    top_module->reset = 0;
    printf("[mycpu] Reset done.\n");
}

void print_wb_info(WB_info i)
{
    printf("PC=0x%8.8x, WBEn = %d, WReg = %d, WBValue = 0x%8.8x\n", i.wb_pc, i.wb_ena, i.wb_reg, i.wb_value);
}

int check(WB_info stu, WB_info ref)
{
    int fail = 0;
    if (stu.wb_pc != ref.wb_pc) {
        fail = 1;
    }
    if (stu.wb_ena != ref.wb_ena) {
        fail = 1;
    }
    if (stu.wb_ena == 1) {
        if (stu.wb_reg != ref.wb_reg || stu.wb_value != ref.wb_value) {
            fail = 1;
        }
    }
    if (fail) {
        printf("[difftest] Test Failed!\n");
        printf("=========== Difference ===========\n");
        printf("SIGNAL NAME\tREFERENCE\tMYCPU\n");
        printf("debug_wb_pc\t0x%8.8x\t0x%8.8x\n", ref.wb_pc, stu.wb_pc);
        printf("debug_wb_ena\t%10d\t%10d\n", ref.wb_ena, stu.wb_ena);
        printf("debug_wb_reg\t%10d\t%10d\n", ref.wb_reg, stu.wb_reg);
        printf("debug_wb_value\t0x%8.8x\t0x%8.8x\n", ref.wb_value, stu.wb_value);
        // exit(-1);
    } else {
        printf("=========== info ===========\n");
        printf("SIGNAL NAME\tREFERENCE\tMYCPU\n");
        printf("debug_wb_pc\t0x%8.8x\t0x%8.8x\n", ref.wb_pc, stu.wb_pc);
        printf("debug_wb_ena\t%10d\t%10d\n", ref.wb_ena, stu.wb_ena);
        printf("debug_wb_reg\t%10d\t%10d\n", ref.wb_reg, stu.wb_reg);
        printf("debug_wb_value\t0x%8.8x\t0x%8.8x\n", ref.wb_value, stu.wb_value);
    }

    // std::cout << stu.inst_valid << std::endl;
    return 0;
}

int main(int argc, char** argv, char** env)
{

    /* ---------- 初始化 ---------- */

    // top = new TESTBENCH<VminiRV_SoC>;
    top = new TESTBENCH();
    char dir[1024] = "waveform/";
    if (argc < 2 || strlen(argv[1]) > 1000) {
        printf("Bad waveform dest path.");
        exit(-1);
    }
    top->opentrace(strcat(strcat(dir, argv[1]), ".vcd"));
    init_cpu(); /* PATH 是 makefile 中传入的 */
    top_module = top->dut;

    /* ---------- 开始仿真 ---------- */

    reset_all();

    top->tick();
    top->tick();
    top->tick();

    printf("[difftest] Test Start!\n");
    WB_info rtl_wb_info, model_wb_info;
    for (int i = 0; i < 100000; i++) {
        rtl_wb_info = top->tick();
        if (rtl_wb_info.wb_have_inst) // 只有这里有指令了, cpu_run_once 才会执行一次
        {
            if (check(rtl_wb_info, cpu_run_once()) == -1) {
                break;
            }
        }
    }
    printf("Timed out! Please check whether your CPU got stuck.\n");
    delete top;
    return 0;
}
