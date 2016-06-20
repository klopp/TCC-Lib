/*
 *  Created on: 20 июня 2016 г.
 *      Author: Vsevolod Lutovinov <klopp@yandex.ru>
 */

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
#include "tcc-lib.h"
#include "tcc.h"
#include <stdarg.h>

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
static void _TccErrorHandler( void *opaque, const char *msg ) {
    strncpy( ( ( TccLib * ) opaque )->error, msg, TCC_ERROR_BUF_SIZE - 1 );
    ( ( TccLib * ) opaque )->error[TCC_ERROR_BUF_SIZE] = 0;
}

TccLib *TccLibInit( TccLib *tcc ) {
    memset( tcc, 0, sizeof( TccLib ) );
    tcc->ts = tcc_new();
    if( !tcc->ts ) {
        return NULL;
    }
    tcc_set_error_func( tcc->ts, tcc, _TccErrorHandler );
    tcc_set_output_type( tcc->ts, TCC_OUTPUT_MEMORY );
    return tcc;
}

void TccLibDown( TccLib *tcc ) {
    tcc_delete( tcc->ts );
}

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
static int _TccLibCallMain( TccLib *tcc ) {
    int size;
    void *mem;
    main_func pmain;
    int rc;
    size = tcc_relocate( tcc->ts, NULL );
    if( size == -1 ) {
        return -1;
    }
    mem = tcc_malloc( size );
    if( !mem ) {
        return -2;
    }
    if( tcc_relocate( tcc->ts, mem ) == -1 ) {
        tcc_free( mem );
        return -3;
    }
    pmain = ( main_func )tcc_get_symbol( tcc->ts, "main" );
    if( !pmain ) {
        tcc_free( mem );
        return -4;
    }
    rc = pmain();
    tcc_free( mem );
    return rc;
}

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
int TccLibMainFromFile( TccLib *tcc, const char *file ) {
    return TccLibLoadFiles( tcc, file, NULL ) ? 1 : _TccLibCallMain( tcc );
}

int TccLibMainFromSource( TccLib *tcc, const char *source ) {
    return TccLibLoadSources( tcc, source, NULL ) ? 1 : _TccLibCallMain( tcc );
}

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
int TccLibLoadFiles( TccLib *tcc, const char *file, ... ) {
    va_list ap;
    const char *current = file;
    va_start( ap, file );
    while( current ) {
        if( tcc_add_file( tcc->ts, current ) == -1 ) {
            va_end( ap );
            return 1;
        }
        current = va_arg( ap, char * );
    }
    va_end( ap );
    return 0;
}

int TccLibLoadSources( TccLib *tcc, const char *source, ... ) {
    va_list ap;
    const char *current = source;
    va_start( ap, source );
    while( current ) {
        if( tcc_compile_string( tcc->ts, current ) ) {
            va_end( ap );
            return 1;
        }
        current = va_arg( ap, char * );
    }
    va_end( ap );
    return 0;
}

//TCC_OUTPUT_PREPROCESS
