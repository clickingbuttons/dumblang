#include "elf.h"
#include <errno.h>
#include <string.h>

#define MAX(a, b) (a > b ? a : b)
#define CHECK_WRITE(file, data, len) if (fwrite(data, 1, len, file) != len) return errno;
#define CHECK_ERRNO(x) if (x != 0) return errno;

int elf_init(Elf* ctx, const char* fname) {
	ctx->out = fopen(fname, "w");
	if (ctx->out == NULL) return errno;

	// skip header, we'll come back to it in `deinit`
	size_t header_len = ctx->is32 ? sizeof(Elf32_Ehdr) : sizeof(Elf64_Ehdr);
	CHECK_ERRNO(fseek(ctx->out, header_len, SEEK_SET));

	return 0;
}

int elf_deinit(Elf* ctx) {
	char buf[MAX(sizeof(Elf32_Ehdr), sizeof(Elf64_Ehdr))] = {
		ELFMAG0,
		ELFMAG1,
		ELFMAG2,
		ELFMAG3,
		ctx->is32 ? ELFCLASS32 : ELFCLASS64,
		ELFDATA2LSB,
		EV_CURRENT,
		ELFOSABI_SYSV,
	};
	Elf64_Ehdr* header64 = (Elf64_Ehdr*)&buf;

	header64->e_type = ET_EXEC;
	header64->e_machine = ctx->is32 ? EM_386 : EM_X86_64;
	header64->e_version = EV_CURRENT;

	Elf64_Off entry = ctx->virtual_addr + ctx->program_header;
	if (ctx->is32) {
		Elf32_Ehdr* header32 = (Elf32_Ehdr*)&buf;
		header32->e_entry = entry + sizeof(Elf32_Phdr);
		header32->e_phoff = ctx->program_header;
		header32->e_ehsize = sizeof(Elf32_Ehdr);
		header32->e_phentsize = sizeof(Elf32_Phdr);
		header32->e_phnum = 1;
	}
	else {
		header64->e_entry = entry + sizeof(Elf64_Phdr);
		header64->e_phoff = ctx->program_header;
		header64->e_ehsize = sizeof(Elf64_Ehdr);
		header64->e_phentsize = sizeof(Elf64_Phdr);
		header64->e_phnum = 1;
	}

	CHECK_ERRNO(fseek(ctx->out, 0, SEEK_SET));
	CHECK_WRITE(ctx->out, buf, ctx->is32 ? sizeof(Elf32_Ehdr) : sizeof(Elf64_Ehdr));
	CHECK_ERRNO(fclose(ctx->out));

	return 0;
}

int elf_write_program(Elf* ctx, void* data, size_t data_len) {
	// Only one program allowed.
	if (ctx->program_header != 0) return 1;
	ctx->program_header = ftell(ctx->out);
	if (ctx->program_header == -1) return errno;

	if (ctx->is32) {
		Elf32_Off fsize = sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) + data_len;
		Elf32_Phdr header = {
			.p_type = PT_LOAD,			/* Segment type */
			.p_offset = 0,		/* Segment file offset */
			.p_vaddr = ctx->virtual_addr,		/* Segment virtual address */
			.p_paddr = ctx->virtual_addr,		/* Segment physical address */
			.p_filesz = fsize,		/* Segment size in file */
			.p_memsz = fsize,		/* Segment size in memory */
			.p_flags = PF_R  | PF_X,		/* Segment flags */
			.p_align = ctx->alignment,		/* Segment alignment */
		};
		CHECK_WRITE(ctx->out, &header, sizeof(Elf32_Phdr));
	} else {
		Elf64_Off fsize = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + data_len;
		Elf64_Phdr header = {
			.p_type = PT_LOAD,			/* Segment type */
			.p_flags = PF_R  | PF_X,		/* Segment flags */
			.p_offset = 0,		/* Segment file offset */
			.p_vaddr = ctx->virtual_addr,		/* Segment virtual address */
			.p_paddr = ctx->virtual_addr,		/* Segment physical address */
			.p_filesz = fsize,		/* Segment size in file */
			.p_memsz = fsize,		/* Segment size in memory */
			.p_align = ctx->alignment,		/* Segment alignment */
		};
		CHECK_WRITE(ctx->out, &header, sizeof(Elf64_Phdr));
	}
	CHECK_WRITE(ctx->out, data, data_len);

	return 0;
}
