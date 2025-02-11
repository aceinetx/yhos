/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
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

void *memcpy(void *dest, void *src, dword len) {
  char *d = dest;
  char *s = src;
  while (len--)
    *d++ = *s++;
  return dest;
}

void *memset(void *dest, dword val, dword len) {
  char *ptr = (char *)dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}

dword strnlen(char *s, dword maxlen) {
  dword i;

  for (i = 0; i < maxlen; ++i)
    if (s[i] == '\0')
      break;
  return i;
}

dword strlen(char *s) {
  dword i;

  for (i = 0; 1; ++i)
    if (s[i] == '\0')
      break;
  return i;
}

char *strncpy(char *s1, char *s2, dword n) {
  dword size = strnlen(s2, n);
  if (size != n)
    memset(s1 + size, '\0', n - size);
  return memcpy(s1, s2, size);
}
