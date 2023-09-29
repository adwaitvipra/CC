/*
 * TRANSLATION UNIT WITH DEFINATIONS OF METHODS
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <assert.h>

#include "lexizer.h"


const char *tokens [] =
{
	"keyword",
	"identifier",
	"constant",
	"string-literal",
	"punctuator",
	"undefined"
};

const char *keywords [] =
{
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"inline", "int", "long", "register",
	"restrict", "return", "short", "signed",
	"sizeof", "static", "struct", "switch",
	"typedef", "union", "unsigned", "void",
	"volatile", "while", "_Alignas", "_Alignof",
	"_Atomic", "_Bool", "_Complex", "_Generic",
	"_Imaginary", "_Noreturn", "_Static_assert", "_Thread_local"
};

const char *preprocessing_keywords [] =
{
	"if", "ifdef", "ifndef",

	"elif", "else", "endif",

	"include", "define", "undef",

	"line", "error", "pragma"
};

const char *punctuators [] =
{
	"[", "]", "(", ")", "{", "}", ".", "->",

	"++", "--", "&", "*", "+", "-", "~", "!",

	"/", "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "^", "|", "&&", "||",

	"?", ":", ";", "...",

	"=", "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|=",

	",", "#", "##",

	"<:", ":>", "<%", "%>", "%:", "%:%:",
};

char *buffer = NULL;

static void print_keywords (void);
static void print_ppkeywords (void);
static void print_punctuators (void);

bool iskeyword (const char *);
bool isppkeyword (const char *);
bool ispunctuator (const char *);

bool isbin (const char *);
bool isoct (const char *);
bool isint (const char *);
bool ishex (const char *);

bool isinteger (const char *);
bool isreal (const char *);

bool isstring (const char *);
bool ischaracter (const char *);

bool isidentifier (const char *);

void *getvalue (struct token_t *, bool);


bool iskeyword (const char *str)
{
	bool flag = false;
	int len, size = sizeof (keywords) / sizeof (const char *);

	if (str && ((len = strlen (str)) > 0))
		for (int idx = 0; (idx < size) && !flag; idx++)
			if (!strncmp (str, keywords[idx], len))
				flag = true;

	return flag;
}

bool isppkeyword (const char *str)
{
	bool flag = false;
	int len, size = sizeof (preprocessing_keywords) / sizeof (const char *);

	if (str && ((len = strlen (str)) > 0))
		for (int idx = 0; (idx < size) && !flag; idx++)
			if (!strncmp (str, preprocessing_keywords[idx], len))
				flag = true;

	return flag;
}

bool ispunctuator (const char *str)
{
	bool flag = false;
	int len, size = sizeof (punctuators) / sizeof (const char *);

	if (str && ((len = strlen (str)) > 0))
		for (int idx = 0; (idx < size) && !flag; idx++)
			if (!strncmp (str, punctuators[idx], len))
				flag = true;

	return flag;
}

bool isbin (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 2))
	{
		flag = true;

		if ((str[0] != '0')
				&& ((str[1] != 'B') && (str[1] != 'b')))
			flag = false;
		else
		{
			for (int idx = 2; (idx < len) && flag; idx++)
				if (str[idx] != '0' && str[idx] != '1')
					flag = false;
		}
	}

	return flag;
}

bool isoct (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 0))
	{
		flag = true;

		if (str[0] != '0')
			flag = false;
		else
		{
			for (int idx = 1; (idx < len) && flag; idx++)
				if (!isdigit (str[idx]) || (str[idx] == '8') || (str[idx] == '9'))
					flag = false;
		}
	}

	return flag;
}

bool isint (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 0))
	{
		flag = true;

		for (int idx = 0; (idx < len) && flag; idx++)
			if (!isdigit (str[idx]))
				flag = false;
	}

	return flag;
}

bool ishex (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 2))
	{
		flag = true;

		if ((str[0] != '0')
				&& ((str[1] != 'X') && (str[1] != 'x')))
			flag = false;
		else
		{
			for (int idx = 2; (idx < len) && flag; idx++)
				if (!isxdigit (str[idx]))
					flag = false;
		}
	}

	return flag;
}

/* TODO: consideration of integer suffix */
bool isinteger (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 0))
	{
		flag = true;

		if (!isbin (str) && !isoct (str) && !isint (str) && !ishex (str))
			flag = false;
	}

	return flag;
}

/* TODO: consideration of prefix */
bool ischaracter (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 0))
		if ((str[0] == '\'') && (str[len - 1] == '\''))
			flag = true;

	return flag;
}

/* TODO: consideration of floating suffix */
bool isreal (const char *str)
{
	int len;

	bool check = false, flag = false;

	char *expox = NULL, *expoy = NULL, *nsign = NULL, *psign = NULL;
	char *tmp = NULL, *period = NULL, *exponent = NULL, *sign = NULL;

	if (str && ((len = strlen (str)) > 0))
	{
		/* at most one period expected, first and last period should be same */
		period = ((tmp = strchr (str, '.')) == strrchr (str, '.')) ? tmp : NULL;

		/* at most one exponent expected, first and last exponent should be same */
		expox = (((tmp = strchr (str, 'E')) == strrchr (str, 'E')) && tmp) ? tmp : NULL;
		expoy = (((tmp = strchr (str, 'e')) == strrchr (str, 'e')) && tmp) ? tmp : NULL;

		if ((expox && !expoy) || (!expox && expoy))
		{
			if (expox)
				exponent = expox;
			else
			{
				exponent = expoy;
			}
		}

		if (exponent)
		{
			/* ignore prefix sign for time being */
			nsign = (((tmp = strchr (str + 1, '-')) == strrchr (str + 1, '-')) && tmp) ? tmp : NULL;
			psign = (((tmp = strchr (str + 1, '+')) == strrchr (str + 1, '+')) && tmp) ? tmp : NULL;

			if ((nsign && !psign) || (!nsign && psign))
			{
				if (nsign)
					sign = nsign;
				else
				{
					sign = psign;
				}
			}
			else
			{
				sign = NULL;
			}
		}

		if (period || exponent)
		{
			if (period && !exponent)
			{
				check = true;

				for (int idx = 0; (idx < len) && check; idx++)
					if (!isdigit (str[idx]) && ((str + idx) != period))
						check = false;
			}
			else if (!period && exponent)
			{

				if ((exponent > str) && (exponent < (str + len - 1)) && (sign < (str + len - 1)))
				{
					if (((!sign) && isdigit (*(exponent + 1))) || ((sign) && isdigit (*(sign + 1))))
					{
						check = true;

						for (int idx = 0; (idx < len) && check; idx++)
							if (!isdigit (str[idx]) && ((str + idx) != exponent)
									&& ((str + idx) != sign))
								check = false;
					}
				}
			}
			else
			{
				if ((exponent > period) && (exponent != (str + 1))
						&& (exponent < (str + len - 1)) && (sign < (str + len - 1)))
				{
					if ((!sign && isdigit (*(exponent + 1))) || (sign && isdigit (*(sign + 1))))
					{
						check = true;

						for (int idx = 0; (idx < len) && check; idx++)
							if (!isdigit (str[idx]) && ((str + idx) != period)
									&& ((str + idx) != exponent) && ((str + idx) != sign))
								check = false;
					}
				}
			}

			if (check)
			{
				flag = true;
			}
		}
	}

	return flag;
}

/* TODO: consideration of encoding prefix */
bool isstring (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 0))
		if ((str[0] == '\"') && (str[len - 1] == '\"'))
			flag = true;

	return flag;
}

bool isidentifier (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 0))
	{
		flag = true;

		if (!isalpha (str[0]) && (str[0] != '_'))
			flag = false;
		else
		{
			for (int idx = 1; idx < len; idx++)
				if (!isalnum (str[idx]) && (str[idx] != '_'))
					flag = false;
		}
	}

	return flag;
}

void *getvalue (struct token_t *token, bool print)
{
	void *retptr = NULL;
	int tag, primary, secondary;

	if (token)
	{
		tag = token->type;
		primary = token->flag[0];
		secondary = token->flag[1];

		if ((primary >= C_INT) && (primary <= C_STR)
				&& (tag >= KEYWD) && (tag <= PUNCT))
		{
			switch (primary)
			{
				case C_INT :
					{
						switch (secondary)
						{
							case INT_C :
								{
									if (print)
									{
										printf ("%d", token->value.const_d.int_c);
									}

									retptr = (void *) &token->value.const_d.int_c;

									break;
								}

							case UINT_C :
								{
									if (print)
									{
										printf ("%u", token->value.const_d.uint_c);
									}

									retptr = (void *) &token->value.const_d.uint_c;

									break;
								}

							case LONG_C :
								{
									if (print)
									{
										printf ("%ld", token->value.const_d.long_c);
									}

									retptr = (void *) &token->value.const_d.long_c;

									break;
								}

							case ULONG_C :
								{
									if (print)
									{
										printf ("%lu", token->value.const_d.ulong_c);
									}

									retptr = (void *) &token->value.const_d.ulong_c;

									break;
								}

							default :
								{
									if (print)
									{
										printf ("error processing constant value\n");
									}

									break;
								}
						}

						break;
					}

				case C_FLT :
					{
						switch (secondary)
						{
							case FLT_C :
								{
									if (print)
									{
										printf ("%f", token->value.const_f.float_c);
									}

									retptr = (void *) &token->value.const_f.float_c;

									break;
								}

							case DBL_C :
								{
									if (print)
									{
										printf ("%f", token->value.const_f.double_c);
									}

									retptr = (void *) &token->value.const_f.double_c;

									break;
								}

							case LDBL_C :
								{
									if (print)
									{
										printf ("%Lf", token->value.const_f.ldouble_c);
									}

									retptr = (void *) &token->value.const_f.ldouble_c;

									break;
								}

							default :
								{
									if (print)
									{
										printf ("error processing constant value\n");
									}

									break;
								}
						}

						break;
					}

				case C_CHR :
					{
						switch (secondary)
						{
							case CHR_C :
								{
									if (print)
									{
										printf ("%c", token->value.const_c.char_c);
									}

									retptr = (void *) &token->value.const_c.char_c;

									break;
								}

							case WCHR_C :
								{
									if (print)
									{
										printf ("%lc", token->value.const_c.wchar_c);
									}

									retptr = (void *) &token->value.const_c.wchar_c;

									break;
								}

							default :
								{
									if (print)
									{
										printf ("error processing constant value\n");
									}

									break;
								}
						}

						break;
					}

				case C_STR :
					{
						switch (secondary)
						{
							case STR_C :
								{
									if (print)
									{
										printf ("%s", token->value.const_s.str_c);
									}

									retptr = (void *) &token->value.const_s.str_c;

									break;
								}

							case WSTR_C :
								{
									if (print)
									{
										printf ("%ls", token->value.const_s.wstr_c);
									}

									retptr = (void *) &token->value.const_s.wstr_c;

									break;
								}

							default :
								{
									if (print)
									{
										printf ("error processing constant value\n");
									}

									break;
								}
						}

						break;
					}

				default :
					{
						if (print)
						{
							printf ("error processing constant value\n");
						}

						break;
					}
			}
		}
	}

	return retptr;
}

struct token_t *resettoken (struct token_t **token)
{
	struct token_t *tok = NULL;

	if (token && (tok = *token))
	{
		tok->type = UNDEF;

		bzero ((void *) &(tok->value), sizeof (tok->value));
		tok->flag[0] = tok->flag[1] = -1;

		bzero ((void *) (tok->lexeme), sizeof (tok->lexeme));
	}

	return tok;
}

struct token_t *settoken (struct token_t **token, enum tag_t type, char *lexeme)
{
	struct token_t *tok = NULL;

	if (token && (tok = *token) && ((type >= KEYWD) && (type <= UNDEF)) && lexeme)
	{
		tok->type = type;
	}

	return tok;
}

struct token_t *gettoken (struct token_t **token)
{
	struct token_t *new = NULL;

	if ((new = (struct token_t *) malloc (sizeof (struct token_t))))
	{
		resettoken (&new);

		if (token)
		{
			*token = new;
		}
	}

	return new;
}

bool scantoken (FILE *fh, struct token_t *token)
{
	bool flag = false;

	if (fh && !feof (fh))
	{
		if (!buffer)
		{
			int curr, max, tmp;
			max = curr = 0;

			tmp = getc (fh);
			while (tmp != EOF)
			{
				while (tmp != '\n')
				{
					curr += 1;
					tmp = getc (fh);
				}

				if (curr > max)
				{
					max = curr;
				}
			}

			max += 1;

			if (!(buffer = (char *) calloc (max, sizeof (char))))
			{
				fprintf (stderr, "Memory allocation failed\n");
			}

			rewind (fh);
		}

		if (buffer && token)
		{
			flag = true;
		}
		else
		{
			fprintf (stderr, "Internal procedure failed\n");
		}
	}

	return flag;
}

static void print_keywords (void)
{
	int sz = sizeof (keywords) / sizeof (const char *);

	printf ("\n");
	printf ("*****************************************************************************************\n*");
	printf ("\t\t\t\t\tKEYWORDS\t\t\t\t\t*\n*");
	printf ("****************************************************************************************\n*");

	for (int idx = 0; idx < sz; idx++)
		printf (" %02d) %-16s%s", idx + 1, keywords[idx], ((idx + 1) % 4)? " " : "*\n*");

	printf ("****************************************************************************************\n");
	printf ("\n");

	return ;
}

static void print_ppkeywords (void)
{
	int sz = sizeof (preprocessing_keywords) / sizeof (const char *);

	printf ("\n");
	printf ("**********************************************************\n*");
	printf ("\t\tPREPROCESSING KEYWORDS\t\t\t *\n*");
	printf ("*********************************************************\n*");

	for (int idx = 0; idx < sz; idx++)
		printf ("  %02d) %-12s%s", idx + 1, preprocessing_keywords[idx], ((idx + 1) % 3) ? " " : "*\n*");

	printf ("*********************************************************\n");
	printf ("\n");

	return ;
}

static void print_punctuators (void)
{
	int sz = sizeof (punctuators) / sizeof (const char *);

	printf ("\n");
	printf ("*************************************************************************************\n*");
	printf ("\t\t\t\tPUNCTUATORS\t\t\t\t\t    *\n*");
	printf ("************************************************************************************\n*");

	for (int idx = 0; idx < sz; idx++)
		printf (" %02d) %-8s%s", idx + 1, punctuators[idx], ((idx + 1) % 6) ? " " : "*\n*");

	printf ("************************************************************************************\n");
	printf ("\n");

	return ;
}
