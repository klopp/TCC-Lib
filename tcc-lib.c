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
static void _TccLibSaveStdOut( TccLib *tcc ) {
    tcc->saved_stdout = dup( fileno( stdout ) );
    tcc->dev_null = fopen( TCC_DEV_NULL, "w" );
    dup2( fileno( tcc->dev_null ), fileno( stdout ) );
    *tcc->error = 0;
}

static void _TccLibRestoreStdOut( TccLib *tcc ) {
    fflush( stdout );
    if( tcc->dev_null ) {
        fclose( tcc->dev_null );
        tcc->dev_null = NULL;
    }
    if( tcc->saved_stdout != -1 ) {
        dup2( tcc->saved_stdout, fileno( stdout ) );
        close( tcc->saved_stdout );
        tcc->saved_stdout = -1;
    }
}

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
static void _TccErrorHandler( void *opaque, const char *msg ) {
    strncpy( ( ( TccLib * ) opaque )->error, msg, TCC_ERROR_BUF_SIZE - 1 );
    ( ( TccLib * ) opaque )->error[TCC_ERROR_BUF_SIZE] = 0;
}

TccLib *TccLibInit( TccLib *tcc ) {
    memset( tcc, 0, sizeof( TccLib ) );
    tcc->saved_stdout = -1;
    tcc->ts = tcc_new();
    if( !tcc->ts ) {
        return NULL;
    }
    tcc_set_error_func( tcc->ts, tcc, _TccErrorHandler );
    tcc_set_output_type( tcc->ts, TCC_OUTPUT_MEMORY );
    return tcc;
}

void TccLibDown( TccLib *tcc ) {
    _TccLibRestoreStdOut( tcc );
    tcc_delete( tcc->ts );
}

TccLib *TccLibReset( TccLib *tcc ) {
    TccLibDown( tcc );
    return TccLibInit( tcc );
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
    pmain = ( main_func ) tcc_get_symbol( tcc->ts, "main" );
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
    return TccLibLoadSourceFiles( tcc, file, NULL ) ? 1 : _TccLibCallMain( tcc );
}

int TccLibMainFromSource( TccLib *tcc, const char *source ) {
    return TccLibLoadSources( tcc, source, NULL ) ? 1 : _TccLibCallMain( tcc );
}

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
int TccLibLoadSourceFiles( TccLib *tcc, const char *file, ... ) {
    va_list ap;
    const char *current = file;
    _TccLibSaveStdOut( tcc );
    va_start( ap, file );
    while( current ) {
        FILE *f = fopen( file, "r" );
        sprintf( tcc->error, "Can not open file \"%s\": %s", current,
                 strerror( errno ) );
        if( f ) {
            int size = 0;
            fseek( f, 0, SEEK_END );
            size = ( int ) ftell( f );
            fseek( f, 0, SEEK_SET );
            if( size ) {
                sprintf( tcc->error, "No memory to read file \"%s\"", current );
                char *mem = tcc_malloc( size + 4 );
                if( mem ) {
                    sprintf( tcc->error, "Error reading file \"%s\": %s",
                             current, strerror( errno ) );
                    if( fread( mem, 1, size, f ) == size ) {
                        strcat( mem, "\n" );
                        if( !tcc_compile_string( tcc->ts, mem ) ) {
                            free( mem );
                            fclose( f );
                            current = va_arg( ap, char * );
                            *tcc->error = 0;
                            continue;
                        }
                    }
                    tcc_free( mem );
                }
            }
            fclose( f );
        }
        va_end( ap );
        _TccLibRestoreStdOut( tcc );
        return -1;
    }
    va_end( ap );
    _TccLibRestoreStdOut( tcc );
    return 0;
}

int TccLibLoadFiles( TccLib *tcc, const char *file, ... ) {
    va_list ap;
    const char *current = file;
    _TccLibSaveStdOut( tcc );
    va_start( ap, file );
    while( current ) {
        if( tcc_add_file( tcc->ts, current ) == -1 ) {
            va_end( ap );
            _TccLibRestoreStdOut( tcc );
            return -1;
        }
        current = va_arg( ap, char * );
    }
    va_end( ap );
    _TccLibRestoreStdOut( tcc );
    return 0;
}

int TccLibLoadSources( TccLib *tcc, const char *source, ... ) {
    va_list ap;
    const char *current = source;
    _TccLibSaveStdOut( tcc );
    va_start( ap, source );
    while( current ) {
        if( tcc_compile_string( tcc->ts, current ) ) {
            va_end( ap );
            _TccLibRestoreStdOut( tcc );
            return -1;
        }
        current = va_arg( ap, char * );
    }
    va_end( ap );
    _TccLibRestoreStdOut( tcc );
    return 0;
}

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
int TccLibBind( TccLib *tcc, const char *name, void *val ) {
    _TccLibSaveStdOut( tcc );
    tcc_add_symbol( tcc->ts, name, val );
    _TccLibRestoreStdOut( tcc );
    return *tcc->error ? -1 : 0;
}

/* -----------------------------------------------------------------------------
 *
 -----------------------------------------------------------------------------*/
