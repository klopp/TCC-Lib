/*
 *  Created on: 20 июня 2016 г.
 *      Author: Vsevolod Lutovinov <klopp@yandex.ru>
 */

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
#include "tcc-lib.h"

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
static void _TccErrorHandler( void *opaque, const char *msg ) {
    strncpy( ( ( TccLib * )opaque )->error, msg, TCC_ERROR_BUF_SIZE - 1 );
    ( ( TccLib * )opaque )->error[TCC_ERROR_BUF_SIZE] = 0;
}

TccLib *TccLibInit( TccLib *tcc ) {
    memset( tcc, 0, sizeof( TccLib ) );
    tcc->ts = tcc_new();
    if( !tcc->ts ) {
        return NULL;
    }
    tcc_set_error_func( tcc->ts, tcc, _TccErrorHandler );
    return tcc;
}

void TccLibDown( TccLib *tcc ) {
    tcc_delete( tcc->ts );
}

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/

