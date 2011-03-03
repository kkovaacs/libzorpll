#include <zorp/misc.h>
#include <string.h>
#include <stdlib.h>

void
testcase(char *pw, char *salt, char *expected)
{
  char result[128];
  
  z_crypt(pw, salt, result, sizeof(result));
  if  (strcmp(result, expected) != 0)
    exit(1);
    
}

int 
main(void)
{
  testcase("titkos", "$1$abcdef$", "$1$abcdef$tViuCKijOibTb1mxJ.nuL1");
  testcase("titkos", "$1$abc$", "$1$abc$.CtgYDt9Kysbluq2wuHVL0");
  testcase("titkos", "$1$abc$", "$1$abc$.CtgYDt9Kysbluq2wuHVL0");
  testcase("titkos", "$1$01234567$", "$1$01234567$8.GchdyyhO1de8.vYREOZ1");
  testcase("titkos", "$1$0123456789$", "$1$01234567$8.GchdyyhO1de8.vYREOZ1");
  
  return 0;
}
