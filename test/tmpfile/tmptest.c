/* 16.05.2007 - Vlad: created */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern int errno;

int
main(void)
{
	char *fname;
	char *temp_fname;
	int len, fd;
	FILE *f;

	fname = malloc(100);
	if (fname == 0) {
		perror("malloc");
		return errno;
	}

	printf("Introduceti prefixul fisierului: "); fflush(stdout);
	fgets(fname, 100, stdin);

	/* 
	 * I find strlen() a must in such situations. Any other
	 * suggestions?
	 */
	len = strlen(fname);
	fname[len - 1] = 0;
	len += 6;
	/*
	 * Trebuie testat daca s-a putut face realocarea.
	 * Stiu ca nu e ceva important, dar in unele cazuri
	 * poate fi, si e bine de stiut.
	 */
	temp_fname = realloc(fname, len);
	if (temp_fname == 0) {
		perror("realloc");
		return errno;
	}
	fname = temp_fname;
	strncat(fname, "XXXXXX", len);

	/*
	 * Using mkstemp() here. tmpfile() deletes the file after we
	 * close it, so it's practically useless for our purpose, and
	 * mktemp() is very badly advertised in the manual.
	 */
	fd = mkstemp(fname);
	if (fd == -1) {
		perror("mkstemp");
		return errno;
	}

	/* 
	 * fdopen() associates an already open file descriptor with a
	 * new FILE * structure. It closes the file descriptor in the
	 * process. Thus, we avoid working with lower level unistd.h
	 * functions _and_ regular stdio.h ones.
	 */
	f = fdopen(fd, "w");
	if (f == 0) {
		perror("fdopen");
		return errno;
	}

	fprintf(f, "Hello tmpfile world!\n");

	printf("Fisierul temporar se numeste: %s\n", fname);
	printf("Dati un ls sau ceva sa va convingeti, "
			"apoi enter sa vedem daca ramane...\n");
	/* 
	 * Buffer was probably not flushed, so don't get worked up like
	 * I did if the file is empty. After closing it, the content
	 * should be there.
	 */
	getchar();

	fclose(f);
	return 0;
}
