#include "link/elf.h"
#include <stdio.h>
#include <stdlib.h>

#define union_init(tag, value) {  tag, { .tag = value } }
#define COMMA ,
#define CHECK(x) if (x) { \
	perror(#x); \
	exit(1);  \
}

int main(int argc, char* argv[]) {
	Elf linker = { 0 };
	linker.virtual_addr = 0x08048000;
	linker.alignment = 0x1000;

	CHECK(elf_init(&linker, "b.out"));
	if (linker.is32) {
		const char program[] = {0xb3, 0x2a, 0x31, 0xc0, 0x40, 0xcd, 0x80};
		CHECK(elf_write_program(&linker, (void*)program, sizeof(program)));
	} else {
		const char program[] = {0xb8, 0x3c, 0, 0, 0, 0xbf, 0x2a, 0, 0, 0, 0x0f, 0x05};
		CHECK(elf_write_program(&linker, (void*)program, sizeof(program)));
	}
	CHECK(elf_deinit(&linker));

	// codegen_x86_64(
	// 	&elf,
	// 	sample_vars,
	// 	ARRAY_LENGTH(sample_vars),
	// 	sample_instructions,
	// 	ARRAY_LENGTH(sample_instructions)
	// );
	return 0;
}
