/* basic wc in lex */
%{
	#include <stdio.h>
	#include <stdbool.h>

	bool flag = false;
	int char_cnt = 0, word_cnt = 0, line_cnt = 0;
%}

%%

[ \f\n\r\t\v] {
	if (flag)
	{
		flag = false;
		word_cnt += 1;
	}

	if ((*yytext) == '\n')
	{
		line_cnt += 1;
	}
	char_cnt += 1;
}

[^ \f\n\r\t\v] {
	if (!flag)
	{ flag = true;}
	char_cnt += 1;
}

%%

int main (const int argc, const char *argv[])
{
	char *filename = NULL;
	FILE *fh = NULL;

	if ((argc > 0)
			&& (fh = fopen ((filename = (char *) argv[1]), "r")))
		yyin = fh;
	else
	{	
		filename = "stdin";
		yyin = stdin;
	}

	yylex ();

	printf ("\t%d\t%d\t%d\t%s\n",
			line_cnt, word_cnt, char_cnt, filename);

	return 0;
}
