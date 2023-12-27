#include <stdio.h>

#define SEN "işeyaramaz"

int main( int argsayi, char *arglar[ ])
{ 
  const char* bazi_sey = arglar[ 0];
  printf( "%s", bazi_sey);

  return 0;
}

