/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#include <kernel/syscall.h>
#include <kernel/yalloc.h>

typedef struct header {
	dword size; // Size includes header and footer, LSB as in-use flag
} header_t;

typedef struct footer {
	dword size;
} footer_t;

typedef struct free_block {
	header_t header;
	struct free_block* next;
	struct free_block* prev;
	// Footer is at the end of the block
} free_block_t;

free_block_t* free_list_head = NULL;
void* heap_start = NULL;
void* heap_end = NULL;

void init_allocator(void* start, dword size) {
	heap_start = start;
	heap_end = (char*)start + size;

	header_t* header = (header_t*)start;
	header->size = size;
	footer_t* footer = (footer_t*)((char*)start + size - sizeof(footer_t));
	footer->size = size;

	free_list_head = (free_block_t*)header;
	free_list_head->next = NULL;
	free_list_head->prev = NULL;
}

void* yalloc(dword size) {
	// Align size to 8 bytes
	size = (size + 7) & ~7;
	dword total_needed = sizeof(header_t) + size + sizeof(footer_t);

	free_block_t* best = NULL;
	for (free_block_t* curr = free_list_head; curr; curr = curr->next) {
		dword curr_size = curr->header.size & ~1;
		if (curr_size >= total_needed && (!best || curr_size < best->header.size))
			best = curr;
	}

	if (!best)
		return NULL;

	dword remaining = (best->header.size & ~1) - total_needed;
	if (remaining >= sizeof(header_t) + sizeof(footer_t) + 8) {
		// Split block
		header_t* new_header = (header_t*)((char*)best + total_needed);
		new_header->size = remaining | 0;
		footer_t* new_footer = (footer_t*)((char*)new_header + remaining - sizeof(footer_t));
		new_footer->size = remaining;

		free_block_t* new_block = (free_block_t*)new_header;
		new_block->next = best->next;
		new_block->prev = best->prev;
		if (best->prev)
			best->prev->next = new_block;
		if (best->next)
			best->next->prev = new_block;
		if (best == free_list_head)
			free_list_head = new_block;

		best->header.size = total_needed | 1;
		footer_t* best_footer = (footer_t*)((char*)best + total_needed - sizeof(footer_t));
		best_footer->size = best->header.size;
	} else {
		// Take entire block
		best->header.size |= 1;
		if (best->prev)
			best->prev->next = best->next;
		if (best->next)
			best->next->prev = best->prev;
		if (best == free_list_head)
			free_list_head = best->next;
	}

	// syscall(SYS_WRITE, "yalloc()\n");
	return (char*)best + sizeof(header_t);
}

void yfree(void* ptr) {
	if (!ptr)
		return;

	header_t* header = (header_t*)((char*)ptr - sizeof(header_t));
	dword size = header->size & ~1;
	header->size &= ~1;

	// Coalesce next
	header_t* next = (header_t*)((char*)header + size);
	if ((char*)next < (char*)heap_end && !(next->size & 1)) {
		size += next->size & ~1;
		free_block_t* next_block = (free_block_t*)next;
		if (next_block->prev)
			next_block->prev->next = next_block->next;
		if (next_block->next)
			next_block->next->prev = next_block->prev;
		if (next_block == free_list_head)
			free_list_head = next_block->next;
	}

	// Coalesce previous
	footer_t* prev_footer = (footer_t*)((char*)header - sizeof(footer_t));
	if ((char*)prev_footer >= (char*)heap_start) {
		header_t* prev = (header_t*)((char*)prev_footer - (prev_footer->size & ~1) + sizeof(footer_t));
		if (!(prev->size & 1)) {
			size += prev->size & ~1;
			free_block_t* prev_block = (free_block_t*)prev;
			if (prev_block->prev)
				prev_block->prev->next = prev_block->next;
			if (prev_block->next)
				prev_block->next->prev = prev_block->prev;
			if (prev_block == free_list_head)
				free_list_head = prev_block->next;
			header = prev;
		}
	}

	// Update header and footer
	header->size = size;
	footer_t* footer = (footer_t*)((char*)header + size - sizeof(footer_t));
	footer->size = size;

	// Add to free list
	free_block_t* block = (free_block_t*)header;
	block->next = free_list_head;
	block->prev = NULL;
	if (free_list_head)
		free_list_head->prev = block;
	free_list_head = block;
	// syscall(SYS_WRITE, "yfree()\n");
}
