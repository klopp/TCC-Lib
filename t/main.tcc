extern int x;
extern int y;
extern int z;

extern int aaa;
extern char bbb[];

int main()
{
  hello();

  y += 10; z += 100; x += 1000;

  printf( "%d, %s\n", aaa, bbb );

  zzz(22,33);

  bye();

  aaa = 666;
  *bbb = 'Z';

  return 0;
}