// This allows expanding macros somewhat recursively to enable expressive
// one-liners.
// https://mailund.dk/posts/macro-metaprogramming/
// Yeah, it's ugly, but it beats all other enum preprocessing I could find.
#pragma once

#include <string.h>

#define expand_1(...) __VA_ARGS__
#define expand_2(...) expand_1(expand_1(__VA_ARGS__))
#define expand_4(...) expand_2(expand_2(__VA_ARGS__))
#define expand_8(...) expand_4(expand_4(__VA_ARGS__))
#define expand_16(...) expand_8(expand_8(__VA_ARGS__))
#define expand_32(...) expand_16(expand_16(__VA_ARGS__))
#define expand_64(...) expand_32(expand_32(__VA_ARGS__))
#define eval expand_64

#define cat(a,b) a##b
#define not(b) test(cat(not_,b))
#define not_0 is_true()
#define truthiness(b) not(not(b))
#define if_else(b) if_else_(truthiness(b))
#define if_else_(b) cat(if_,b)
#define if_0(...) else_0
#define if_1(...) __VA_ARGS__ else_1
#define else_0(...) __VA_ARGS__
#define else_1(...)

#define second(a, b, ...) b
#define test(p) second(p, 0)
#define is_true() -, 1

#define head(a, ...) a
#define heads(a, ...) #a
#define tail(a, ...) __VA_ARGS__
#define is_nil(...) test(head(is_true __VA_ARGS__)())
#define is_nils(...) test(heads(is_true __VA_ARGS__)())

#define empty()
#define delay(f) f empty()
#define delay2(f) f empty empty()()
#define map_(f, ...) \
	if_else(is_nil(__VA_ARGS__)) \
	() \
	(f(head(__VA_ARGS__)) delay2(map__)()(f, tail(__VA_ARGS__)))
#define map__() map_
#define map(f, ...) eval(map_(f, __VA_ARGS__))

#define catcomma(a,b) #a,
#define STRING_ELE(ENUM) catcomma(ENUM, COMMA)

#define COUNT_OF(a) (sizeof a / sizeof a[0])

#define ENUM(name, ...) \
	typedef enum name { __VA_ARGS__ } name; \
	const char* name##_strings[] = { map(STRING_ELE, __VA_ARGS__) }; \
	static name string_to_##name(const char* s) { \
		for (int i = 0; i < COUNT_OF(name##_strings); i++) { \
			if (strcmp(name##_strings[i], s) == 0) return i; \
		} \
		return -1; \
	}
