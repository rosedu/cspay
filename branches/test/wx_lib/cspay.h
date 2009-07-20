/*
 * RD: header-ele linux, glibc, etc. folosesc ceva de genul
 * #ifndef _CSPAY_H
 * #define _CSPAY_H	1
 *
 * ar trebui sa ne orientam si noi la fel, nu?
 */

#ifndef __CSPAY_H__
#define __CSPAY_H__

#ifdef __cplusplus
extern "C" {
#endif

	float media(int, int, int);
	char *mesaj(void);
#ifdef __cplusplus
}
#endif

#endif /* __CSPAY_H__ */
