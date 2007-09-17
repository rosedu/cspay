/*
 * libspreadconv debug header, suggested by Razvan
 * First draft: 21.07.2997, Vlad Dogaru
 */

/**
 * \ingroup libspreadconv
 * \file debug.h
 * debug header
 * \author Vlad Dogaru
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

/*
 * A problem arises when you use Dprintf with a single argument;
 * for example
 * 	Dprintf ("I like variadic macros");
 *
 * Fortunately, GNU CPP has a way out of this with the '##' token
 * paste operator.
 * (http://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html)
 *
 * All hail GNU CPP.
 */

#if defined (__DEBUG__)
#define Dprintf(format, ...) \
	fprintf(stderr, "[%s]:%s:%d: " format, __FILE__, \
		__func__, __LINE__, ##__VA_ARGS__)
#else
#define Dprintf(format, ...)
#endif /* DEBUG*/

#endif /* _DEBUG_H_ */
