#include "libtcc.c"

#include "tcc-lib.h"

int main() {
    TccLib tcc;
    int rc = 0;
    int aaa = 99;
    char bbb[] = "bBb";
    /*
     *
     */
    TccLibInit( &tcc );
    rc = TccLibLoadFiles( &tcc, "./t/hello-bye.c", NULL );
    if( rc ) {
        printf( "1) rc = %d, %s\n", rc, tcc.error );
        TccLibDown( &tcc );
        return rc;
    }
    rc = TccLibBind( &tcc, "aaa", &aaa );
    rc = TccLibBind( &tcc, "aaa", &aaa );
    if( rc ) {
        printf( "3) rc = %d, %s\n", rc, tcc.error );
        TccLibDown( &tcc );
        return rc;
    }
    tcc_add_symbol( tcc.ts, "bbb", &bbb );
    /*
     *
     */
    rc = TccLibMainFromFile( &tcc, "./t/main.c" );
    printf( "2) rc = %d, %s\n", rc, tcc.error );
    /*
     *
     */
    TccLibDown( &tcc );
    return rc;
}
