#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct Linker {
	char* data;
	char* bss;
	char* text;

	int (*emit_data)(struct Linker* ctx, void* ptr, size_t len);
	int (*emit_bss)(struct Linker* ctx, void* ptr, size_t len);
	int (*emit_text)(struct Linker* ctx, void* ptr, size_t len);
} Linker;
