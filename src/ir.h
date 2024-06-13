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
} ir_Type;

typedef struct {
	enum {
		add,
		cast,
	} tag;
	union {
		ir_Type to;
	} val;
} ir_Op;

typedef struct {
	ir_Type tag;
	union {
		uint8_t  u8;
		uint16_t  u16;
		uint32_t  u32;
		uint64_t  u64;
		float f;
		double d;
		size_t ptr;
	} value;
} ir_Immediate;

typedef uint8_t ir_Register;

typedef struct {
	enum {
		register_,
		memory,
		immediate,
	} tag;
	union {
		ir_Register reg;
		// TODO: memory
		ir_Immediate immediate;
	} arg;
} ir_Arg;

typedef struct {
	ir_Op op;
	ir_Arg dst;
	ir_Arg src;
} ir_Instruction;

typedef struct {
	const char* name;
	ir_Immediate default_;
} ir_Variable;

typedef struct {
	ir_Variable* vars;
	ir_Instruction* instructions;
} ir_Function;
