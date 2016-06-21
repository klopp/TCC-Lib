/*
 *  Created on: 20 июня 2016 г.
 *      Author: Vsevolod Lutovinov <klopp@yandex.ru>
 */

#ifndef TCC_LIB_H_
#define TCC_LIB_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <linux/limits.h>
#include "tcc.h"

#define TCC_ERROR_BUF_SIZE  PATH_MAX+PATH_MAX
#ifdef __unix
# define TCC_DEV_NULL "/dev/null"
#else
# define TCC_DEV_NULL "NUL"
#endif

typedef struct _TccLib {
    TCCState *ts;
    char error[TCC_ERROR_BUF_SIZE];
    int saved_stdout;
    FILE *dev_null;
} TccLib;

typedef int ( *main_func )();

TccLib *TccLibInit( TccLib *tcc );
void TccLibDown( TccLib *tcc );
TccLib *TccLibReset( TccLib *tcc );

int TccLibLoadFiles( TccLib *tcc, const char *file, ... );
int TccLibLoadSources( TccLib *tcc, const char *source, ... );
int TccLibLoadSourceFiles( TccLib *tcc, const char *file, ... );

int TccLibMainFromFile( TccLib *tcc, const char *file );
int TccLibMainFromSource( TccLib *tcc, const char *source );

int TccLibBind( TccLib *tcc, const char *name, void *val );

#if defined(__cplusplus)
}; /* extern "C" */
#endif

#endif /* TCC_LIB_H_ */

/*
 *  That's All, Folks!
 */
