/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */

/*
 * This is a helper gcc that automatically sets up needed arguments
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef SOURCE_PATH
#error "SOURCE_PATH is not set, make sure you are compiling with makefiles"
#endif

int main(int argc, char **argv) {
  // Allocate space for the new arguments:
  char **newargv = malloc((sizeof(char *) * 4) * (argc + 2));
  if (!newargv) {
    perror("malloc");
    return EXIT_FAILURE;
  }

  newargv[0] = "i386-elf-gcc";
  newargv[1] = "-ffreestanding";
  newargv[2] = "-m64";
  newargv[3] = "-nostdlib";

  char ld_arg[1024];
  snprintf(ld_arg, 1024, "-T%s/yhse.ld", SOURCE_PATH);

  // If we are not compiling to a object file, add a -T argument
  bool isObj = false;
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-c") == 0) {
      newargv[4] = ld_arg;
      isObj = true;
    }
  }

  if (!isObj) {
    printf("i386-yhse-gcc: warning: you are compiling & linking, this may or "
           "may not "
           "work depending on your linker's standard libraries\n");
  }

  // Copy the provided arguments to the new array.
  for (int i = 1; i < argc; i++) {
    newargv[i + 1] = argv[i];
  }
  newargv[argc + 1] = NULL;

  // Execute ld with the new argument list.
  execvp("i386-elf-gcc", newargv);
  free(newargv);

  // If execvp returns, an error occurred.
  perror("execvp");
  return EXIT_FAILURE;
}
