#include <zorp/random.h>

#include <stdio.h>

#define ROUNDS 1000

int 
main(void)
{
  guchar buf[64];
  guint i, j;
  
  for (i = 0; i < ROUNDS; i++)
    {
      z_random_sequence_get_bounded(Z_RANDOM_STRONG, buf, sizeof(buf), 'A', 'Z');
      for (j = 0; j < sizeof(buf); j++)
        {
          if (buf[j] < 'A' || buf[j] > 'Z')
            {
              fprintf(stderr, "Invalid character in bounded random data: %02x [%c]\n", (guint) buf[j], buf[j]);
              return 1;
            }
        }
        
    }
  return 0;
}
