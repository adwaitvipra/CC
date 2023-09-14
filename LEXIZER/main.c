#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lexizer.h"

#define SIZE 128
#define FDEF "output.lx"
#define LX ".lx"

int main (const int argc, const char *argv[])
{
	int retval = EXIT_FAILURE;

	size_t cnt;
	struct stat statbuf;

	const char *fdef = FDEF;
	char *fin = NULL, *fout = NULL, *fincp = NULL;
	char buffer[SIZE];

	FILE *fhin = NULL, *fhout = NULL;

	if (argc > 1)
	{
		fin = (char *) argv[1];

		if (argc > 2)
			fout = (char *) argv[2];
		else
		{
			if ((fincp = (char *) malloc ((strlen (fin) + 2) * sizeof (char )))
					&& (fincp = strcpy (fincp, fin)))
			{
				fincp = (char *) strtok (fincp, ".");
				fincp = strcat (fincp, LX);

				fout = (char *) fincp;
			}
			else
			{
				fout = (char *) fdef;
			}
		}


	}

	if (fin && fout && !stat (fin, &statbuf)
			&& (fhin = fopen (fin, "r")) && (fhout = fopen (fout, "w")))
	{

		while ((cnt = fread (buffer, sizeof (char), SIZE, fhin)) > 0)
		{
			fwrite (buffer, sizeof (char), cnt, fhout);
		}

		if (!fclose (fhin) && !fclose (fhout))
		{
			retval = EXIT_SUCCESS;
		}
	}

	if (fincp)
	{
		free (fincp);
		fincp = NULL;
	}

	return retval;
}
