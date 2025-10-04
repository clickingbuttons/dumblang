#include "elf.h"
#include "../error.h"
#include <stddef.h>
#include <string.h>

#define Ehdr Elf64_Ehdr
#define Phdr Elf64_Phdr
#define Xword Elf64_Xword

int elf64_init(Elf64 *ctx, const char *fname) {
	ctx->out = fopen(fname, "w");
	if (ctx->out == NULL)
		return errno;
	CHECK(fseek(ctx->out, sizeof(Ehdr), SEEK_SET));
	return 0;
}

int elf64_write_program(Elf64 *ctx, void *data, size_t data_len) {
	if (ctx->program_header != 0)
		return 1;
	ctx->program_header = ftell(ctx->out);
	if (ctx->program_header == -1)
		return errno;

	Xword fsize = data_len + sizeof(Ehdr) + sizeof(Phdr);
	Phdr header = {
		.p_type = PT_LOAD,
		.p_flags = PF_R | PF_X,
		.p_offset = 0,
		.p_vaddr = ctx->virtual_addr,
		.p_paddr = ctx->virtual_addr,
		.p_filesz = fsize,
		.p_memsz = fsize,
		.p_align = ctx->alignment,
	};
	CHECK(fwrite(&header, sizeof(header), 1, ctx->out) != 1);
	CHECK(fwrite(data, data_len, 1, ctx->out) != 1);
	return 0;
}

int elf64_deinit(Elf64 *ctx) {
	CHECK(fseek(ctx->out, 0, SEEK_SET));
	Ehdr header = {
		.e_ident =
			{
				ELFMAG0,
				ELFMAG1,
				ELFMAG2,
				ELFMAG3,
				ELFCLASS64, // ELFCLASS32,
				ELFDATA2LSB,
				EV_CURRENT,
				ELFOSABI_SYSV,
			},
		.e_type = ET_EXEC,
		.e_machine = EM_X86_64, // EM_386
		.e_version = EV_CURRENT,
		.e_phoff = ctx->program_header,
		.e_entry = ctx->virtual_addr + ctx->program_header + sizeof(Phdr),
		.e_ehsize = sizeof(Ehdr),
		.e_phentsize = sizeof(Phdr),
		.e_phnum = 1,
	};
	CHECK(fwrite(&header, sizeof(header), 1, ctx->out) != 1);
	CHECK(fclose(ctx->out));
	return 0;
}
