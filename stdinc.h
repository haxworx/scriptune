#ifndef __stdinc_h__
#define __stdinc_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PROGRAM_NAME "scriptune"

char *strdup(const char *s);
void sc_error(char *msg);
void sc_error_fmt(const char *format, ...);
void sc_error_clean(char *msg);
void show_help(void);
char *make_full_path(char *book, char *prefix);
#endif
