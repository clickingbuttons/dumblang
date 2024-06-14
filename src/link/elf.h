#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "../../vendor/elf.h"

typedef struct {
	FILE* out;
	Elf64_Off virtual_addr;
	Elf64_Off program_header;
	bool is32;
	Elf64_Word alignment;
} Elf;

int elf_init(Elf* ctx, const char* fname);
int elf_deinit(Elf* ctx);
int elf_write_program(Elf* ctx, void* data, size_t data_len);
