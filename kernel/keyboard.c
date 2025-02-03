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
    res.ch = 'A';
    return res;
  }
  if (input == 0x30) {
    res.ch = 'B';
    return res;
  }
  if (input == 0x2E) {
    res.ch = 'C';
    return res;
  }
  if (input == 0x20) {
    res.ch = 'D';
    return res;
  }
  if (input == 0x12) {
    res.ch = 'E';
    return res;
  }
  if (input == 0x21) {
    res.ch = 'F';
    return res;
  }
  if (input == 0x22) {
    res.ch = 'G';
    return res;
  }
  if (input == 0x23) {
    res.ch = 'H';
    return res;
  }
  if (input == 0x17) {
    res.ch = 'I';
    return res;
  }
  if (input == 0x24) {
    res.ch = 'J';
    return res;
  }
  if (input == 0x25) {
    res.ch = 'K';
    return res;
  }
  if (input == 0x26) {
    res.ch = 'L';
    return res;
  }
  if (input == 0x32) {
    res.ch = 'M';
    return res;
  }
  if (input == 0x31) {
    res.ch = 'N';
    return res;
  }
  if (input == 0x18) {
    res.ch = 'O';
    return res;
  }
  if (input == 0x19) {
    res.ch = 'P';
    return res;
  }
  if (input == 0x10) {
    res.ch = 'Q';
    return res;
  }
  if (input == 0x13) {
    res.ch = 'R';
    return res;
  }
  if (input == 0x1F) {
    res.ch = 'S';
    return res;
  }
  if (input == 0x14) {
    res.ch = 'T';
    return res;
  }
  if (input == 0x16) {
    res.ch = 'U';
    return res;
  }
  if (input == 0x2F) {
    res.ch = 'V';
    return res;
  }
  if (input == 0x11) {
    res.ch = 'W';
    return res;
  }
  if (input == 0x2d) {
    res.ch = 'X';
    return res;
  }
  if (input == 0x15) {
    res.ch = 'Y';
    return res;
  }
  if (input == 0x2c) {
    res.ch = 'Z';
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
  res.ch = input;
  res.is_valid = 0;
  return res;
}
