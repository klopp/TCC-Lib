int x = 1;
int y = 1;
int z = 1;

void hello()
{
  printf( "[%u/%u] Hello, Tcc-Lib!\n", y++,z++ );
}

void bye()
{
  printf( "[%u/%u] Good bye, Tcc-Lib!\n", y++,z++ );
}