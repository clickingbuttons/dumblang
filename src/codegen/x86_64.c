#include <stdio.h>
#include "x86_64.h"

typedef enum {
	RAX,
} Register;

int gen_instruction(
	Linker* linker,
	Instruction* instruction
) {
	uint8_t encoded[16];

	return 0;
}

int codegen_x86_64(
	Linker* linker,
	Variable* variables,
	size_t n_variables,
	Instruction* instructions,
	size_t n_instructions
) {
	// Register registers[MAX_VARIABLES];

	// assign to registers
	// registers[0] = RAX;
	// for (Variable* variable = variables; variable != NULL; variable++) {
	// 	switch (variable->type) {
	// 	case memory:
	// 		puts("TODO: memory");
	// 		exit(1);
	// 		break;
	// 	}
	// }

	// emit data + bss

	// emit instructions
	for (size_t i = 0; i < n_instructions; i++) {
		Instruction* instruction = instructions + i;
		switch (instruction->op.tag) {
		case mov: {
			uint8_t encoded[] = { 0xb8, 0x07, 0x00, 0x00, 0x00 };
			linker->emit_text(linker, encoded, ARRAY_LENGTH(encoded));
			break;
		}
		default:
			printf("unknown instruction %d\n", instruction->op.tag);
			return 1;
		}
	}

	uint8_t ret[] = { 0xc3 };
	linker->emit_text(linker, ret, ARRAY_LENGTH(ret));

	return 0;
}
