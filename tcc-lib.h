/*
 *  Created on: 20 июня 2016 г.
 *      Author: Vsevolod Lutovinov <klopp@yandex.ru>
 */

/*
 *  That's All, Folks!
 */
#ifndef TCC_LIB_H_
#define TCC_LIB_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <linux/limits.h>
#include <unistd.h>
#include <string.h>
#include "../tcc/libtcc.h"

#define TCC_ERROR_BUF_SIZE  PATH_MAX+PATH_MAX

typedef struct _TccLib {
    TCCState *ts;
    char error[TCC_ERROR_BUF_SIZE];
} TccLib;

TccLib *TccLibInit( TccLib *pc );
void TccLibDown( TccLib *pc );
TccLib *TccLibReset( TccLib *pc );


#if defined(__cplusplus)
}; /* extern "C" */
#endif


#endif /* TCC_LIB_H_ */
