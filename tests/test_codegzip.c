#include <zorp/code_gzip.h>
#include <stdlib.h>

#include <stdio.h>
#include <string.h>

int main()
{
  gchar buf[4096], buf2[4096];
  gchar *compressed = NULL;
  gsize length = 0;
  ZCode *gz;
  gint i, j;

  memset(buf, 'A', sizeof(buf));
  memset(buf2, 'B', sizeof(buf2));
  gz = z_code_gzip_encode_new(1024, 1);
  
  length = 0;
  for (i = 0; i < 4096; i++)
    {
      if (!z_code_transform(gz, buf, sizeof(buf)) ||
          !z_code_transform(gz, buf2, sizeof(buf2)) ||
          !z_code_finish(gz))
        {
          fprintf(stderr, "Compression failed\n");
          return 1;
        }
      compressed = realloc(compressed, length + z_code_get_result_length(gz));
      memcpy(compressed + length, z_code_peek_result(gz), z_code_get_result_length(gz));
      length += z_code_get_result_length(gz);
      z_code_flush_result(gz, 0);
    }

  gz = z_code_gzip_decode_new(4096);
  if (!z_code_transform(gz, compressed, length) ||
      !z_code_finish(gz))
    {
      fprintf(stderr, "Decompression failed\n");
      return 1;
    }
  if (z_code_get_result_length(gz) != 4096 * 2 * sizeof(buf))
    {
      fprintf(stderr, "Decompression resulted different length, than compressed length='%d', result='%d' ???\n", length, z_code_get_result_length(gz));
      return 1;
    }
  for (i = 0; i < 4096; i++)
    {
      for (j = 0; j < 2; j++)
        {
          guchar check;
          
          memset(buf, 'C', sizeof(buf));
          length = z_code_get_result(gz, buf, sizeof(buf));
          if (length != sizeof(buf))
            {
              fprintf(stderr, "Expected some more data\n");
              return 1;
            }
          if (j == 0)
            check = 'A';
          else 
            check = 'B';
          for (i = 0; i < sizeof(buf); i++)
            {
              if (buf[i] != check)
                {
                  fprintf(stderr, "Invalid data returned from decompression\n");
                  return 1;
                }
            }
        }
    
    }
  
  return 0;
}
