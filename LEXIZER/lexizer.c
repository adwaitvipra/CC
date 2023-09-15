/*
 * TRANSLATION UNIT WITH DEFINATIONS OF METHODS
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "lexizer.h"

const char *classes [] =
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

static void print_keywords (void);
static void print_ppkeywords (void);
static void print_punctuators (void);

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

	if (str && ((len = strlen (str)) > 1))
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

bool isdec (const char *str)
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

bool isinteger (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 0))
	{
		flag = true;

		if (!isbin (str) && !isoct (str) && !isdec (str) && !ishex (str))
			flag = false;
	}

	return flag;
}


bool ischaracter (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 0))
		if ((str[0] == '\'') && (str[len - 1] == '\''))
			flag = true;

	return flag;
}

bool isreal (const char *str)
{
	int len;
	bool flag = false;

	if (str && ((len = strlen (str)) > 0))
	{
		flag = true;
	}

	return flag;
}

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
