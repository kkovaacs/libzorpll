#include <zorp/registry.h>

#include <stdio.h>

int main(void)
{
  int i;
  char buf[128];
  
  z_registry_init();
  
  for (i = 0; i < 10; i++)
    {
      snprintf(buf, sizeof(buf), "key%d", i);
      z_registry_add(buf, ZR_PROXY, (gpointer) i);
    }
  for (i = 0; i < 10; i++) 
    {
      gint res;
      gint type = 0;
      
      snprintf(buf, sizeof(buf), "key%d", i);
      res = GPOINTER_TO_UINT(z_registry_get(buf, &type));
      if (res != i)
        {
          printf("problem.\n");
          return 1;
        }
      if (type != ZR_PROXY)
        {
          printf("problem 2.\n");
          return 1;
        }
    }
  
  z_registry_destroy();
  return 0;
}
