/**
 * @file onboard.c 这是一些外设
 * @author wangfiox (wangfiox@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "onboard.h"

uint32_t digit_value;

uint32_t read_seven_seg(uint32_t rel_addr, AccessMode mode)
{
  panic("7-segment display cannot be read.");
}

void write_seven_seg(uint32_t rel_addr, AccessMode mode, uint32_t data)
{
  Assert(mode == ACCESS_WORD && rel_addr == 0, "Access violation");
  digit_value = data;
  printf("Digit: 0x%x\n", digit_value);
}

uint32_t read_keyboard(uint32_t rel_addr, AccessMode mode) { panic("TODO"); }

void write_keyboard(uint32_t rel_addr, AccessMode mode, uint32_t data)
{
  panic("TODO");
}

uint32_t read_led(uint32_t rel_addr, AccessMode mode) { panic("TODO"); }

void write_led(uint32_t rel_addr, AccessMode mode, uint32_t data)
{
  panic("TODO");
}

uint32_t read_switch(uint32_t rel_addr, AccessMode mode) { panic("TODO"); }

void write_switch(uint32_t rel_addr, AccessMode mode, uint32_t data)
{
  panic("TODO");
}

uint32_t read_button(uint32_t rel_addr, AccessMode mode) { panic("TODO"); }

void write_button(uint32_t rel_addr, AccessMode mode, uint32_t data)
{
  panic("TODO");
}