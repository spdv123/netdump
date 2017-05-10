#include <sys/types.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/stat.h>

#include <ctype.h>
#include <errno.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include "pcap.h"
#include <stdio.h>
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <stdlib.h>
#include <string.h>
#ifdef TIME_WITH_SYS_TIME
#include <time.h>
#endif
#include <unistd.h>

extern char *program_name;

/* VARARGS */
#if __STDC__
void error(const char *fmt, ...)
#else
error(fmt, va_alist)
	const char *fmt;
	va_dcl
#endif
{
	va_list ap;

	(void)fprintf(stderr, "%s: ", program_name);
#if __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	if (*fmt) {
		fmt += strlen(fmt);
		if (fmt[-1] != '\n')
			(void)fputc('\n', stderr);
	}
	exit(1);
	/* NOTREACHED */
}

/* VARARGS */
void
#if __STDC__
warning(const char *fmt, ...)
#else
warning(fmt, va_alist)
	const char *fmt;
	va_dcl
#endif
{
	va_list ap;

	(void)fprintf(stderr, "%s: WARNING: ", program_name);
#if __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	if (*fmt) {
		fmt += strlen(fmt);
		if (fmt[-1] != '\n')
			(void)fputc('\n', stderr);
	}
}

/*
 * Copy arg vector into a new buffer, concatenating arguments with spaces.
 */
char *
copy_argv(register char **argv)
{
	register char **p;				// what is "register char"? (refer to "C Language Keywords")
	register u_int len = 0;			// what is "register u_int"? (refer to "C Language Keywords")
	char *buf;
	char *src, *dst;

	p = argv;
	if (*p == 0)
		return 0;

	while (*p)
		len += strlen(*p++) + 1;	// the number of arguments

	buf = (char *)malloc(len);
	if (buf == NULL)
		error("copy_argv: malloc");

	p = argv;
	dst = buf;
	while ((src = *p++) != NULL) {
		while ((*dst++ = *src++) != '\0')
			;
		dst[-1] = ' ';				// why the index is "-1"?
	}
	dst[-1] = '\0';

	return buf;
}

