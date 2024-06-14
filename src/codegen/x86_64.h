#pragma once
#include "../ir.h"
#include "../link/link.h"

#define MAX_VARIABLES 2048
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

int codegen_x86_64(
	Linker* linker,
	Variable* variables,
	size_t n_variables,
	Instruction* instructions,
	size_t n_instructions
);
