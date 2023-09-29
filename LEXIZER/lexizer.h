/*
 * DEFINATIONS OF DATA STRUCTURES
 * DECLARATIONS OF METHODS FOR PARSING
 */
#ifndef LEXILIZER_H
#define LEXILIZER_H 1

#include <stddef.h>
#include "list.h"

#define MAX_LEN 256

enum flag_t {C_INT, C_FLT, C_CHR, C_STR};
enum flag_d {INT_C, UINT_C, LONG_C, ULONG_C};
enum flag_f {FLT_C, DBL_C, LDBL_C};
enum flag_c {CHR_C, WCHR_C};
enum flag_s {STR_C, WSTR_C};

enum tag_t {KEYWD, IDENT, CONST, STRLIT, PUNCT, UNDEF};

union int_t
{
	int int_c;
	unsigned int uint_c;

	long int long_c;
	unsigned long int ulong_c;
};

union float_t
{
	float float_c;

	double double_c;
	long double ldouble_c;
};

union char_t
{
	char char_c;
	wchar_t wchar_c;
};

union string_t
{
	char *str_c;
	wchar_t *wstr_c;
};

union const_t
{
	union int_t const_d;
	union float_t const_f;

	union char_t const_c;
	union string_t const_s;
};

struct token_t
{
	enum tag_t type;

	int flag[2];
	union const_t value;

	char lexeme[MAX_LEN];
};

extern const char *tokens[];
extern const char *keywords[];
extern const char *preprocessing_keywords[];
extern const char *punctuators[];

#endif /* LEXILIZER_H */
