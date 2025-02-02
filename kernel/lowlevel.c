#include "lowlevel.h"

void outb(word port, byte value) {
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

byte inb(word port) {
  byte value;
  asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}
