#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "../../vendor/elf.h"

typedef struct {
	FILE* out;
	Elf64_Off virtual_addr;
	Elf64_Off program_header;
	Elf64_Word alignment;
} Elf64;
int elf64_init(Elf64 *ctx, const char* fname);
int elf64_deinit(Elf64 *ctx);
int elf64_write_program(Elf64 *ctx, void* data, size_t data_len);
