#include "elf.h"
#include <errno.h>
#include <string.h>
#include <stddef.h>

#define MAX(a, b) (a > b ? a : b)
#define CHECK_WRITE(file, data, len) if (fwrite(data, 1, len, file) != len) return errno;
#define CHECK_ERRNO(x) if (x != 0) return errno;
#define WRITE(is32) { \
	if (is32) WRITE_HEADER(Elf32_Ehdr, Elf32_Phdr) else WRITE_HEADER(Elf64_Ehdr, Elf64_Phdr); \
}

int elf_init(Elf* ctx, const char* fname) {
	ctx->out = fopen(fname, "w");
	if (ctx->out == NULL) return errno;

	// skip header, we'll come back to it in `deinit`
	size_t header_len = ctx->is32 ? sizeof(Elf32_Ehdr) : sizeof(Elf64_Ehdr);
	printf("header_len %lu %lu\n", sizeof(Elf32_Ehdr), sizeof(Elf64_Ehdr));
	CHECK_ERRNO(fseek(ctx->out, header_len, SEEK_SET));

	return 0;
}

int elf_deinit(Elf* ctx) {
	CHECK_ERRNO(fseek(ctx->out, 0, SEEK_SET));

#define WRITE_HEADER(Ehdr, Phdr) { \
	Ehdr header = { \
		.e_ident = { \
			ELFMAG0, \
			ELFMAG1, \
			ELFMAG2, \
			ELFMAG3, \
			ctx->is32 ? ELFCLASS32 : ELFCLASS64, \
			ELFDATA2LSB, \
			EV_CURRENT, \
			ELFOSABI_SYSV, \
		}, \
		.e_type = ET_EXEC, \
		.e_machine = ctx->is32 ? EM_386 : EM_X86_64, \
		.e_version = EV_CURRENT, \
		.e_phoff = ctx->program_header, \
		.e_entry = ctx->virtual_addr + ctx->program_header + sizeof(Phdr), \
		.e_ehsize = sizeof(Ehdr), \
		.e_phentsize = sizeof(Phdr), \
		.e_phnum = 1, \
	}; \
	CHECK_WRITE(ctx->out, &header, sizeof(Ehdr)); \
}
	WRITE(ctx->is32);
#undef WRITE_HEADER

	CHECK_ERRNO(fclose(ctx->out));

	return 0;
}

int elf_write_program(Elf* ctx, void* data, size_t data_len) {
	// Only one program allowed.
	if (ctx->program_header != 0) return 1;
	ctx->program_header = ftell(ctx->out);
	if (ctx->program_header == -1) return errno;

#define WRITE_HEADER(Ehdr, Phdr) { \
	Elf64_Xword fsize = sizeof(Ehdr) + sizeof(Phdr) + data_len; \
	Phdr header = { \
		.p_type = PT_LOAD, \
		.p_flags = PF_R  | PF_X, \
		.p_offset = 0, \
		.p_vaddr = ctx->virtual_addr, \
		.p_paddr = ctx->virtual_addr, \
		.p_filesz = fsize, \
		.p_memsz = fsize, \
		.p_align = ctx->alignment, \
	}; \
	CHECK_WRITE(ctx->out, &header, sizeof(Phdr)); \
}
	WRITE(ctx->is32);
#undef WRITE_HEADER

	CHECK_WRITE(ctx->out, data, data_len);

	return 0;
}
