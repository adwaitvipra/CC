/*
 * DEFINATIONS OF DATA STRUCTURES
 * DECLARATIONS OF METHODS FOR PARSING
 */

#include <stddef.h>

#define MAX_LEN 256

enum classid {KEYWD, IDENT, CONST, STRLIT, PUNCT, UNDEF};

union integer
{
	int int_c;
	unsigned int uint_c;
	long int long_c;
	unsigned long int ulong_c;
};

union character
{
	char char_c;
	wchar_t wchar_c;
};

union real
{
	float float_c;
	double double_c;
	long double ldouble_c;
};

union string
{
	char *str_c;
	wchar_t *wstr_c;
};

union constant
{
	union integer const_int;
	union character const_char;
	union real const_float;
	union string const_string;
};

struct token_t
{
	enum classid type;
	union constant value;
	char token[MAX_LEN];
};

extern const char *classes[];
extern const char *keywords[];
extern const char *preprocessing_keywords[];
extern const char *punctuators[];
