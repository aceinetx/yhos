/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/types.h>

byte keyboard_read_input() {
  while (!(inb(0x64) & 1))
    ;
  return inb(0x60);
}

keyboard_result keyboard_handle_input() {
  uint8_t input = keyboard_read_input();
  keyboard_result res;
  res.is_valid = 1;
  if (input == 0x1E) {
    res.ch = 'a';
    return res;
  }
  if (input == 0x30) {
    res.ch = 'b';
    return res;
  }
  if (input == 0x2E) {
    res.ch = 'c';
    return res;
  }
  if (input == 0x20) {
    res.ch = 'd';
    return res;
  }
  if (input == 0x12) {
    res.ch = 'e';
    return res;
  }
  if (input == 0x21) {
    res.ch = 'f';
    return res;
  }
  if (input == 0x22) {
    res.ch = 'g';
    return res;
  }
  if (input == 0x23) {
    res.ch = 'h';
    return res;
  }
  if (input == 0x17) {
    res.ch = 'i';
    return res;
  }
  if (input == 0x24) {
    res.ch = 'j';
    return res;
  }
  if (input == 0x25) {
    res.ch = 'k';
    return res;
  }
  if (input == 0x26) {
    res.ch = 'l';
    return res;
  }
  if (input == 0x32) {
    res.ch = 'm';
    return res;
  }
  if (input == 0x31) {
    res.ch = 'n';
    return res;
  }
  if (input == 0x18) {
    res.ch = 'o';
    return res;
  }
  if (input == 0x19) {
    res.ch = 'p';
    return res;
  }
  if (input == 0x10) {
    res.ch = 'q';
    return res;
  }
  if (input == 0x13) {
    res.ch = 'r';
    return res;
  }
  if (input == 0x1F) {
    res.ch = 's';
    return res;
  }
  if (input == 0x14) {
    res.ch = 't';
    return res;
  }
  if (input == 0x16) {
    res.ch = 'u';
    return res;
  }
  if (input == 0x2F) {
    res.ch = 'v';
    return res;
  }
  if (input == 0x11) {
    res.ch = 'w';
    return res;
  }
  if (input == 0x2d) {
    res.ch = 'x';
    return res;
  }
  if (input == 0x15) {
    res.ch = 'y';
    return res;
  }
  if (input == 0x2c) {
    res.ch = 'z';
    return res;
  }
  if (input == 0x39) {
    res.ch = ' ';
    return res;
  }
  if (input == 0x1c) {
    res.ch = '\n';
    return res;
  }
  if (input == 0x34) {
    res.ch = '.';
    return res;
  }

  if (input >= 0x02 && input <= 0x0a) {
    res.ch = 49 + input - 0x02;
    return res;
  }

  if (input == 0x0b) {
    res.ch = '0';
    return res;
  }

  if (input == 0x2a) {
    res.is_valid = 1;
    return res;
  }

  res.ch = input;
  res.is_valid = 0;
  return res;
}
