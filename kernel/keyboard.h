#pragma once
#include <kernel/types.h>

typedef struct {
  char ch;
  byte is_valid;
} keyboard_result;

byte keyboard_read_input();
keyboard_result keyboard_handle_input();
