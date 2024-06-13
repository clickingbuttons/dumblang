#include "ir.h"

#define tag_init(tag_, value) {  tag_, { .tag = value } }

ir_Variable sample_vars[] = {
	{ .name = "reg1", .default_ = { .tag = u8, .value = 0 } }
};

ir_Instruction sample_instructions[] = {
	{
		.op = { .tag = add },
		.dst = { .tag = register_, .arg = 1 },
		.src = { .tag = immediate, .arg = { .immediate = { .tag = u8, .value = { .u8 = 1 } } } }
	}
};
