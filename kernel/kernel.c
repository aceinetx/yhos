#include <stdint.h>

void syscall_print(char c) {
  asm volatile("int $0x80"
               :
               : "a"(0), "b"(c) // Syscall 0, character in BL
  );
}

int main() {
  // syscall_print('A'); // Print 'A' via syscall
  for (;;)
    ;
}
