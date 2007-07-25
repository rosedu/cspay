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

#ifndef _SPREADCONV_DEBUG_H_
#define _SPREADCONV_DEBUG_H_

#define DEBUG 		1

#ifdef DEBUG
#define Dprintf(format, ...) \
	fprintf(stderr, "[%s]:%s:%d: " format, __FILE__, \
		__func__, __LINE__, __VA_ARGS__)
#else
#define Dprintf(format, ...)
#endif /* DEBUG*/

#endif /* _SPREADCONV_DEBUG_H_ */
