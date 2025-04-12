/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#pragma once
#include <kernel/types.h>
void init_allocator(void* start, dword size);
void* yalloc(dword size);
void yfree(void* ptr);
