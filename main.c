#include "libtcc.c"

#include "tcc-lib.h"

int main() {
    TccLib tcc;
    TccLibInit( &tcc );
    TccLibDown( &tcc );
    return 0;
}
