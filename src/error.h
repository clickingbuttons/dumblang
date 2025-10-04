#pragma once

#include <errno.h>
#include <stdio.h> // fprintf
#include <stdlib.h> // exit

#define CHECK(x)                                                               \
	if (x) {                                                                     \
		fprintf(stderr, "%s:%d: ", __FILE__, __LINE__);                            \
		if (errno)                                                                 \
			fprintf(stderr, "%s\n", strerror(errno));                                \
		else                                                                       \
			fprintf(stderr, "%s\n", #x);                                             \
		exit(1);                                                                   \
	}                                                                            
