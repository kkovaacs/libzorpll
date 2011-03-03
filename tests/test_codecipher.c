#include <zorp/code_cipher.h>
#include <string.h>

int main()
{
  gchar buf[4097], buf2[4097], key[128], iv[128];
  gchar *encrypted = NULL;
  gsize length = 0;
  ZCode *cipher;
  gint i, j;
  const EVP_CIPHER *algo;
  EVP_CIPHER_CTX cipher_ctx;

  memset(buf, 'A', sizeof(buf));
  memset(buf2, 'B', sizeof(buf2));

  algo = EVP_aes_128_cbc();
  memset(key, '\x55', sizeof(key));
  memset(iv, '\xaa', sizeof(iv));
  g_assert((gint) sizeof(key) > algo->key_len);
  g_assert((gint) sizeof(iv) > algo->iv_len);

  EVP_CipherInit(&cipher_ctx, algo, key, iv, TRUE);
  cipher = z_code_cipher_new(&cipher_ctx);
  
  for (i = 0; i < 4096; i++)
    {
      if (!z_code_transform(cipher, buf, sizeof(buf)) ||
          !z_code_transform(cipher, buf2, sizeof(buf2)))
        {
          fprintf(stderr, "Encryption failed\n");
          return 1;
        }
    }
  if (!z_code_finish(cipher))
    {
          fprintf(stderr, "Encryption failed\n");
          return 1;
    }
  
  length = z_code_get_result_length(cipher);
  encrypted = g_malloc(length);
  memcpy(encrypted, z_code_peek_result(cipher), length);
  z_code_free(cipher);

  EVP_CipherInit(&cipher_ctx, algo, key, iv, FALSE);
  cipher = z_code_cipher_new(&cipher_ctx);
  if (!z_code_transform(cipher, encrypted, length) ||
      !z_code_finish(cipher))
    {
      fprintf(stderr, "Decryption failed\n");
      return 1;
    }
  if (z_code_get_result_length(cipher) != 4096 * 2 * sizeof(buf))
    {
      fprintf(stderr, "Decryption resulted different length, than encrypted length='%d', result='%d' ???\n", 4096 * 2 * sizeof(buf), z_code_get_result_length(cipher));
      return 1;
    }
  for (i = 0; i < 4096; i++)
    {
      for (j = 0; j < 2; j++)
        {
          guchar check;
          
          memset(buf, 'C', sizeof(buf));
          length = z_code_get_result(cipher, buf, sizeof(buf));
          if (length != sizeof(buf))
            {
              fprintf(stderr, "Expected some more data\n");
              return 1;
            }
          if (j == 0)
            check = 'A';
          else 
            check = 'B';
          for (i = 0; i < (gint) sizeof(buf); i++)
            {
              if (buf[i] != check)
                {
                  fprintf(stderr, "Invalid data returned from decryption\n");
                  return 1;
                }
            }
        }
    
    }
  
  return 0;
}
