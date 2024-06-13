#include <stdio.h>

int main(int argc, char* argv[]) {
	puts(
		"export function w $main() {\n"
		"@start\n"
		"	ret 3\n"
		"}"
	);
	return 0;
}
