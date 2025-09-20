# Dumblang

Dumblang is a progressively typed language with comptime. It prioritizes
correctness and readability. It can handle systems programming, data science,
GUI programming, and even GPU programming.

Dumblang is a:
- [ ] Cross-platform assembler
    - [ ] x86
    - [ ] x86_64
    - [ ] arm32
    - [ ] aarch64
- [ ] Cross-platform linker 
    - [ ] Linux 32/64 bit
    - [ ] Mac 32/64 bit
    - [ ] Windows 32/64 bit
- [ ] Test runner
- [ ] Language with interpretted and compiled dialects
- [ ] Package manager
- [ ] Standard library

## Building

- C11 compiler
- Make

### To bootstrap, or not to bootstrap?

Before writing the first Dumblang compiler, I considered whether to quickly
bootstrap it or slowly write a C implementation.

#### Pros:
- Language features
    - Inline tests
    - Pattern matching
        - Exhaustive
    - Tagged unions
    - Interfaces
    - Namespaces
    - Bounds checking
    - Allocation checking
    - Lambdas
    - Defer
    - Range types
    - Comptime
        - Perfect hash table
    - Better type system
    - Error handling
    - Nicer syntax
- Stdlib
    - Container types
    - Hash functions
- Build system

#### Cons
- Debugging until add DWARF
- C code is highly availabile, Dumblang code is not
- Have to maintain a bootstrap process

#### Methods

There are a few ways to bootstrap:

1. Write a Dumblang interpreter in C and then write the compiler in Dumblang
    - Each new language feature used in the compiler must be ported to the C
    interpreter
    - I must resist making the C interpreter nice
    - Bytecode spec may get out-of-sync
2. Write v1 of the compiler in C and then write v2 in Dumblang
    - Builders starting from scratch will have to "chain" compiler builds each
    time new language features are used in the compiler
        - Will need to tag these builds
3. Write v1 in C, v2 in Dumblang, and then replace v1 with v2 "translated" to C
    - Requires C backend
    - Translation will be large and unreadable
    - Translation needs updating for new language feature used in the compiler
4. Write v1 in C, v2 in Dumblang, and then replace v1 with a bytecode
interpreter + blob of v2's bytecode
    - Blob is large and unreadable
    - Blob needs updating for new language feature used in the compiler

I think method #1 is best.

#### Decision

The bootstrap methods are painful to maintain, so the cons outweigh the pros.

I don't think Dumblang is superior enough to C to warrant writing it in itself.
In fact, I'd like to start Dumblang as a C compiler so it can interop with C
and gain access to the incredible amount of quality C already written,
including its own source code.
