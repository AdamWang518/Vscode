#include <stdio.h>

int main (argc, argv)
	int argc;
	char *argv[];
{
	FILE *fp;

	fp = fopen (argv[1], "w");
	fprintf (fp, "Hello!\n");
	fclose (fp);

	return 0;
}//main()



















