#pragma once

#include <stddef.h>
#include <stdint.h>

typedef enum {
	u8,
	u16,
	u32,
	u64,
	i8,
	i16,
	i32,
	i64,
	f32,
	f64,
	ptr,
} Type;

typedef struct {
	Type tag;
	union {
		uint8_t  u8;
		uint16_t  u16;
		uint32_t  u32;
		uint64_t  u64;
		int8_t  i8;
		int16_t i16;
		int32_t i32;
		int64_t i64;
		float f32;
		double f64;
		size_t ptr;
	} value;
} Immediate;

typedef struct {
	const char* name;
	Type type;
} Variable;

typedef struct {
	enum {
		variable,
		memory,
		immediate,
	} tag;
	union {
		Variable variable;
		// TODO: memory
		Immediate immediate;
	} arg;
} Arg;

typedef struct {
	enum {
		mov,
		add,
		cast,
	} tag;
	union {
		struct{} mov;
		struct{} add;
		Type cast;
	} val;
} Op;

typedef struct {
	Op op;
	Arg dst;
	Arg src;
} Instruction;

typedef struct {
	Variable* vars;
	Instruction* instructions;
} Function;
