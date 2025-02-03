#include <kernel/std.h>

int strcmp(char *p1, char *p2) {
  char *s1 = (char *)p1;
  char *s2 = (char *)p2;
  char c1, c2;

  do {
    c1 = (char)*s1++;
    c2 = (char)*s2++;
    if (c1 == '\0')
      return c1 - c2;
  } while (c1 == c2);

  return c1 - c2;
}
