#include "stdinc.h"
#include <stdarg.h>


void show_help(void)
{
	fprintf(stderr, "Usage: %s [OPTION] (ARGUMENTS)\n", PROGRAM_NAME);
	fprintf(stderr, "OPTIONS:\n");
	fprintf(stderr, "    -r         random quote, same as with no arguments or incomplete parameters\n");
	fprintf(stderr, "    -s         display specific verse range\n");
	fprintf(stderr, "               (Bookname) (Chapter) (Starting Verse) (Ending Verse)\n");
	fprintf(stderr, "    -b         (Book) use other books than KJV\n");
	fprintf(stderr, "    -l         list books available\n");
	fprintf(stderr, "    -h         this menu\n");
	fprintf(stderr, "EXAMPLES:\n");
	fprintf(stderr, "    %s -s john1 2 1       # print John I, Chapter 2, Verse 1\n", PROGRAM_NAME);
	fprintf(stderr, "    %s -s matthew 4 3 10  # print Matthew, Chapter 2, Verse 3 to 10\n", PROGRAM_NAME);	
	exit(EXIT_FAILURE);
}



void sc_error(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void sc_error_clean(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

void sc_error_fmt(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}

char *make_full_path(char *prefix, char *book)
{
	static char path[1024];

	sprintf(path, "%s/share/%s/%s", prefix, PROGRAM_NAME, book);

	return path;
}
