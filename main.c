#include "tcc-lib.h"

static int zzz( int a, int b ) {
    return printf( "zzz(%d,%d)\n", a, b );
}

int main() {
    TccLib tcc;
    int rc = 0;
    int aaa = 99;
    char bbb[] = "bBb";
    /*
     *
     */
    TccLibInit( &tcc );
    rc = TccLibLoadSourceFiles( &tcc, "./t/hello-bye.tcc", NULL );
    if( rc ) {
        printf( "1) rc = %d, %s\n", rc, tcc.error );
        TccLibDown( &tcc );
        return rc;
    }
    rc = TccLibBind( &tcc, "aaa", &aaa );
    /*
        rc = TccLibBind( &tcc, "aaa", &aaa );
        if( rc ) {
            printf( "3) rc = %d, %s\n", rc, tcc.error );
            TccLibDown( &tcc );
            return rc;
        }
    */
    TccLibBind( &tcc, "bbb", &bbb );
    TccLibBind( &tcc, "zzz", zzz );
    /*
     *
     */
    rc = TccLibMainFromFile( &tcc, "./t/main.tcc" );
    printf( "2) rc = %d, %s\n", rc, *tcc.error ? tcc.error : "no errors" );
    /*
     *
     */
    printf( "\n---\nAfter external \"main()\" execution:\naaa = %d, bbb = %s\n",
            aaa, bbb );
    /*
     *
     */
    TccLibDown( &tcc );
    return rc;
}
