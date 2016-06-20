#include "libtcc.c"

#include "tcc-lib.h"

int main() {
    TccLib tcc;
    int rc = 0;
    /*
     *
     */
    TccLibInit( &tcc );
    rc = TccLibLoadFiles( &tcc, "./t/main.c", NULL );
    if( rc ) {
        printf( "1) rc = %d, %s\n", rc, tcc.error );
        TccLibDown( &tcc );
        return rc;
    }
    rc = TccLibMainFromFile( &tcc, "./t/main.c" );
    printf( "2) rc = %d, %s\n", rc, tcc.error );
    /*
     *
     */
    TccLibDown( &tcc );
    return rc;
}
