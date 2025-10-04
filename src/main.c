#include "error.h"
#include "link/elf.h"
#include "target.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int ch;

	char *outfile = "a.out";
	// TODO: compile time discovery
	Architecture architecture = DefaultArchitecture;
	OperatingSystem operating_system = DefaultOperatingSystem;
	Abi abi = DefaultAbi;

	while ((ch = getopt(argc, argv, "o:t:")) != -1) {
		switch (ch) {
		case 'o': {
			outfile = optarg;
			break;
		}
		case 't': {
			char *token;
			for (int i = 0; i < 3 && (token = strsep(&optarg, "-")) != NULL; i++) {
				switch (i) {
				case 0:
					architecture = string_to_Architecture(token);
					break;
				case 1:
					operating_system = string_to_OperatingSystem(token);
					if (architecture == -1) {
						fprintf(stderr, "unsupported architecture %s\n", token);
						return 1;
					}
					if (operating_system == -1) {
						fprintf(stderr, "unsupported operating_system %s\n", token);
						return 1;
					}
					if (abi == -1) {
						fprintf(stderr, "unsupported abi %s\n", token);
						return 1;
					}
					break;
				case 2:
					abi = string_to_Abi(token);
					break;
				}
			}
			break;
		}
		default:
			printf("unknown option %c\n", ch);
			return 1;
		}
	}

#define undetected(s)                                                          \
	if (s == -1) {                                                               \
		fprintf(stderr,                                                            \
						"default " #s " wasn't detected at compile-time, specify -t\n");   \
		return -1;                                                                 \
	}
	undetected(architecture);
	undetected(operating_system);
	undetected(abi);
#undef undetected

	switch (operating_system) {
	case linux: {
		switch (architecture) {
		// case x86:
		// 	const char program[] = {0xb3, 0x2a, 0x31, 0xc0, 0x40, 0xcd, 0x80};
		case x86_64: {
			const char program[] = {0xb8, 0x3c, 0, 0, 0,		0xbf,
															0x2a, 0,		0, 0, 0x0f, 0x05};
			Elf64 linker = {
				.virtual_addr = 0x08048000,
				.alignment = 0x1000,
			};
			CHECK(elf64_init(&linker, outfile));
			CHECK(elf64_write_program(&linker, (void *)program, sizeof(program)));
			CHECK(elf64_deinit(&linker));
			break;
		}
		case aarch64: {
			CHECK("no aarch64 support yet");
		}
		}
		break;
	}
	case mac:
		CHECK("no mac support yet");
		break;
	case win:
		CHECK("no windows support yet");
		break;
	}
	CHECK(chmod(outfile, 0755));

	return 0;
}
