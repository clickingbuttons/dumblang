#include "enum.h"
#include "link/elf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define union_init(tag, value)                                                 \
	{                                                                            \
		tag, { .tag = value }                                                      \
	}
#define COMMA ,
#define CHECK(x)                                                               \
	if (x) {                                                                     \
		perror(#x);                                                                \
		exit(1);                                                                   \
	}

// <arch><sub>-<os>-<abi>
ENUM(Architecture, x86, x86_64)
ENUM(OperatingSystem, linux, mac, win)
ENUM(Abi, none)

int main(int argc, char *argv[]) {
	int ch;

	char *outfile = "a.out";
	// TODO: runtime or compile time discovery
	Architecture architecture = -1;
	OperatingSystem operating_system = -1;
	Abi abi = -1;

	while ((ch = getopt(argc, argv, "o:t:")) != -1) {
		switch (ch) {
		case 'o':
			outfile = optarg;
			break;
		case 't':
			char *token;
			for (int i = 0; i < 3 && (token = strsep(&optarg, "-")) != NULL; i++) {
				switch (i) {
				case 0:
					architecture = string_to_Architecture(token);
					if (architecture == -1) {
						printf("unsupported architecture %s\n", token);
						return 1;
					}
					break;
				case 1:
					operating_system = string_to_OperatingSystem(token);
					if (operating_system == -1) {
						printf("unsupported operating_system %s\n", token);
						return 1;
					}
					break;
				case 2:
					abi = string_to_Abi(token);

					if (abi == -1) {
						printf("unsupported abi %s\n", token);
						return 1;
					}
					break;
				}
				break;
			default:
				return 1;
			}
		}
	}

	switch (operating_system) {
	case linux: {
		Elf linker = {
				.virtual_addr = 0x08048000,
				.alignment = 0x1000,
		};
		CHECK(elf_init(&linker, outfile));

		const char *program;

		switch (architecture) {
		case x86: {
			const char generated[] = {0xb3, 0x2a, 0x31, 0xc0, 0x40, 0xcd, 0x80};
			program = generated;
			break;
		}
		case x86_64: {
			const char generated[] = {0xb8, 0x3c, 0, 0, 0,		0xbf,
																0x2a, 0,		0, 0, 0x0f, 0x05};
			program = generated;
			break;
		}
		}

		CHECK(elf_write_program(&linker, (void *)program, sizeof(program)));
		CHECK(elf_deinit(&linker));
		break;
	}
	case mac:
		break;
	case win:
		break;
	}

	return 0;
}
