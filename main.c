#include "libtcc.c"

#include "tcc-lib.h"

int main() {
    TccLib tcc;
    int rc = 0;
    static char source[4096] = { 0 };
    int ( *tmain )() = NULL;
    int size;
    void *mem = NULL;
    /*
     *
     */
    FILE *fin = fopen( "./t/main.tcc", "r" );
    fread( source, 1, sizeof( source ), fin );
    fclose( fin );
    /*
     *
     */
    TccLibInit( &tcc );
    tcc_add_library_path( tcc.ts, "." );
    tcc_add_library_path( tcc.ts, ".." );
    tcc_set_output_type( tcc.ts, TCC_OUTPUT_MEMORY );
    rc = tcc_compile_string( tcc.ts, source );
    printf( "1) rc = %d\n", rc );
    if( rc ) {
        printf( "Error: %s\n", tcc.error );
        TccLibDown( &tcc );
        return rc;
    }
    size = tcc_relocate( tcc.ts, NULL );
    mem = tcc_malloc( size );
    tcc_relocate( tcc.ts, mem );
    tmain = tcc_get_symbol( tcc.ts, "main" );
    if( !tmain ) {
        printf( "Error: %s\n", tcc.error );
    }
    rc = tmain();
    printf( "2) rc = %d\n", rc );
    /*
     *
     */
    TccLibDown( &tcc );
    tcc_free( mem );
    return rc;
}
