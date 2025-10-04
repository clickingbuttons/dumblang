#include "enum.h"

#undef linux
#undef mac
#undef win

// <arch><sub>-<os>-<abi>
ENUM(Architecture, x86_64, aarch64)
ENUM(OperatingSystem, linux, mac, win)
ENUM(Abi, none)

#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__) || defined(_M_X64)
#define DefaultArchitecture x86_64
#elif __aarch64__
#define DefaultArchitecture aarch64
#else
#define DefaultArchitecture -1
#warning Cannot detect default architecture
#endif

#ifdef _WIN64
#define DefaultOperatingSystem win
#elif __APPLE__
#define DefaultOperatingSystem mac
#elif __linux__
#define DefaultOperatingSystem linux
#else
#define DefaultOperatingSystem -1
#warning Cannot detect default OS
#endif

#define DefaultAbi none
