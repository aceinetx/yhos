/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#include <kernel/keyboard.h>
#include <kernel/lowlevel.h>
#include <kernel/types.h>

// Reads one byte from the keyboard controller.
byte keyboard_read_input() {
  // Wait until the output buffer is full.
  while (!(inb(0x64) & 1))
    ;
  return inb(0x60);
}

keyboard_result keyboard_handle_input() {
  // Static variable to track the shift state.
  static int shift_pressed = 0;

  uint8_t input = keyboard_read_input();
  keyboard_result res;
  // Default: mark result as not valid.
  res.is_valid = 0;

  // First, check if this is a key release event.
  if (input & 0x80) {
    uint8_t key_released = input & 0x7F; // Mask out the high bit.
    if (key_released == 0x2A || key_released == 0x36) {
      // Shift key released.
      shift_pressed = 0;
    }
    return res; // Do not produce any character on key release.
  }

  // Check for shift key press events.
  if (input == 0x2A || input == 0x36) {
    shift_pressed = 1;
    return res; // No character output for shift press.
  }

  // Handle backspace
  if (input == 0x0E && !(input & 0x80)) {
    res.ch = 0x0E;    // Return the backspace character.
    res.is_valid = 0; // Mark as not a valid printable key.
    return res;
  }

  // For other keys, mark the result as valid.
  res.is_valid = 1;

  // Letter keys: return uppercase if shift is pressed, lowercase otherwise.
  if (input == 0x1E) { // A
    res.ch = shift_pressed ? 'A' : 'a';
    return res;
  }
  if (input == 0x30) { // B
    res.ch = shift_pressed ? 'B' : 'b';
    return res;
  }
  if (input == 0x2E) { // C
    res.ch = shift_pressed ? 'C' : 'c';
    return res;
  }
  if (input == 0x20) { // D
    res.ch = shift_pressed ? 'D' : 'd';
    return res;
  }
  if (input == 0x12) { // E
    res.ch = shift_pressed ? 'E' : 'e';
    return res;
  }
  if (input == 0x21) { // F
    res.ch = shift_pressed ? 'F' : 'f';
    return res;
  }
  if (input == 0x22) { // G
    res.ch = shift_pressed ? 'G' : 'g';
    return res;
  }
  if (input == 0x23) { // H
    res.ch = shift_pressed ? 'H' : 'h';
    return res;
  }
  if (input == 0x17) { // I
    res.ch = shift_pressed ? 'I' : 'i';
    return res;
  }
  if (input == 0x24) { // J
    res.ch = shift_pressed ? 'J' : 'j';
    return res;
  }
  if (input == 0x25) { // K
    res.ch = shift_pressed ? 'K' : 'k';
    return res;
  }
  if (input == 0x26) { // L
    res.ch = shift_pressed ? 'L' : 'l';
    return res;
  }
  if (input == 0x32) { // M
    res.ch = shift_pressed ? 'M' : 'm';
    return res;
  }
  if (input == 0x31) { // N
    res.ch = shift_pressed ? 'N' : 'n';
    return res;
  }
  if (input == 0x18) { // O
    res.ch = shift_pressed ? 'O' : 'o';
    return res;
  }
  if (input == 0x19) { // P
    res.ch = shift_pressed ? 'P' : 'p';
    return res;
  }
  if (input == 0x10) { // Q
    res.ch = shift_pressed ? 'Q' : 'q';
    return res;
  }
  if (input == 0x13) { // R
    res.ch = shift_pressed ? 'R' : 'r';
    return res;
  }
  if (input == 0x1F) { // S
    res.ch = shift_pressed ? 'S' : 's';
    return res;
  }
  if (input == 0x14) { // T
    res.ch = shift_pressed ? 'T' : 't';
    return res;
  }
  if (input == 0x16) { // U
    res.ch = shift_pressed ? 'U' : 'u';
    return res;
  }
  if (input == 0x2F) { // V
    res.ch = shift_pressed ? 'V' : 'v';
    return res;
  }
  if (input == 0x11) { // W
    res.ch = shift_pressed ? 'W' : 'w';
    return res;
  }
  if (input == 0x2D) { // X
    res.ch = shift_pressed ? 'X' : 'x';
    return res;
  }
  if (input == 0x15) { // Y
    res.ch = shift_pressed ? 'Y' : 'y';
    return res;
  }
  if (input == 0x2C) { // Z
    res.ch = shift_pressed ? 'Z' : 'z';
    return res;
  }

  // Non-letter keys.
  if (input == 0x39) { // Space
    res.ch = ' ';
    return res;
  }
  if (input == 0x1C) { // Enter/newline
    res.ch = '\n';
    return res;
  }
  if (input == 0x34) { // Period
    res.ch = '.';
    return res;
  }

  // Number keys: scancodes 0x02 to 0x0A produce '1' to '9'
  if (input >= 0x02 && input <= 0x0A) {
    res.ch = '1' + (input - 0x02);
    return res;
  }
  if (input == 0x0B) { // 0 key
    res.ch = '0';
    return res;
  }

  // If the scancode is not recognized, mark as invalid.
  res.is_valid = 0;
  return res;
}
