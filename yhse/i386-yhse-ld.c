/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */

/*
 * This is a helper linker that automatically sets up a linker script
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef SOURCE_PATH
#error "SOURCE_PATH is not set, make sure you are compiling with makefiles"
#endif

int main(int argc, char **argv) {
  // Allocate space for the new arguments:
  char **newargv = malloc(sizeof(char *) * (argc + 2));
  if (!newargv) {
    perror("malloc");
    return EXIT_FAILURE;
  }

  newargv[0] = "i386-elf-ld";

  char ld_arg[1024];
  snprintf(ld_arg, 1024, "-T%s/yhse.ld", SOURCE_PATH);
  newargv[1] = ld_arg;

  // Copy the provided arguments to the new array.
  for (int i = 1; i < argc; i++) {
    newargv[i + 1] = argv[i];
  }
  newargv[argc + 1] = NULL;

  // Execute ld with the new argument list.
  execvp("i386-elf-ld", newargv);
  free(newargv);

  // If execvp returns, an error occurred.
  perror("execvp");
  return EXIT_FAILURE;
}
