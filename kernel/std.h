#pragma once
#include <kernel/types.h>

int strcmp(char *s1, char *s2);
void *memcpy(void *dest, void *src, dword len);
void *memset(void *dest, dword val, dword len);
dword strnlen(char *s, dword maxlen);
char *strncpy(char *s1, char *s2, dword n);
