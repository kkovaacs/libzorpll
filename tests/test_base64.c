#include <zorp/code.h>
#include <zorp/code_base64.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int
error(const char *msg)
{
  fprintf(stderr, "%s\n", msg);
  return 1;
}

void
hexdump(const char* buf, int len)
{
  int     i, j;

  for (i = 0; i < len; i += 0x10)
    {
      printf("%06x: ", i);
      for (j = i; j < (i + 0x10); j++)
          if (j < len) printf("%02x ", (int)(unsigned char)buf[j]);
          else printf("   ");
      for (j = i; (j < (i + 0x10)) && (j < len); j++)
          printf("%c", isprint(buf[j]) ? buf[j] : '.');
      printf("\n");
    }
}

int
get_and_dump(ZCode *c, int len, const char *shouldbe, int shouldbelen)
{
  char *dst;
  int i, err;

  i = z_code_get_result_length(c);
  printf("Result length is '%d' bytes, obtaining '%d' bytes\n", i, len);
  dst = (char*)malloc(len);
  len = z_code_get_result(c, dst, len);
  printf("Got '%d' bytes, dump follows:\n", len);
  hexdump(dst, len);
  if (shouldbelen >= 0)
    {
      if (len != shouldbelen)
          return error("Result length mismatch");
    }
  if (shouldbe)
    {
      if (memcmp(dst, shouldbe, len))
          return error("Result mismatch");
    }
  printf("Result matches\n");
  err = z_code_get_errors(c);
  i = z_code_get_result_length(c);
  printf("Errors = '%d', remaining result length is '%d' bytes\n", err, i);
  free(dst);

  return i;
}

int
check_for_error(ZCode *c, const char *src, int srclen, int shall_be_error)
{
  int i;
  int has_error, partly_processed;
  char dummy[1024];
  
  printf("Testing '%s' for errors: ", src);
  z_code_clear_errors(c);
  i = z_code_transform(c, src, srclen);
  i &= z_code_finish(c);
  has_error = !i;
 
  while (z_code_get_result(c, dummy, 1024) == 1024)
      ;
  z_code_clear_errors(c);
 
  if (!shall_be_error != !has_error)
    {
      printf("Unexpected behaviour; has_error='%d', shall_be_error='%d'\n",
             has_error, shall_be_error);
      return 1;
    }

  printf("OK; has_error='%d'\n", has_error);
  return 0;
}

int
main(void)
{
  int i, len;
  unsigned char *src;
  unsigned char q;
  ZCode *enc, *dec, *dec_noerr;
  
  enc = z_code_base64_encode_new(0, 0);
  if (!enc)
      return error("Can't instantiate encoder");
  
  dec = z_code_base64_decode_new(0, FALSE);
  if (!dec)
      return error("Can't instantiate decoder");

  dec_noerr = z_code_base64_decode_new(0, TRUE);
  if (!dec_noerr)
      return error("Can't instantiate decoder");

  /***********************************************************************/
  /* vanilla case, just the basic operation */
  src = "ingyombingyom";
  printf("\nTesting the encoder, transforming '%s'\n", src);
  len = strlen(src);
  i = z_code_transform(enc, src, len);
  printf("Encoded '%d' bytes, closing encoder\n", i);
  z_code_finish(enc);
  get_and_dump(enc, 1024, "aW5neW9tYmluZ3lvbQ==\r\n", 22);

  src = "aW5neW9tYmluZ3lvbQ==";
  printf("\nTesting the decoder, transforming '%s'\n", src);
  len = strlen(src);
  i = z_code_transform(dec, src, len);
  printf("Decoded '%d' bytes, closing decoder\n", i);
  z_code_finish(dec);
  get_and_dump(dec, 1024, "ingyombingyom", 13);

  /***********************************************************************/
  /* partial read */
  src = "ingyombingyom";
  printf("\nTesting partial read (encoder side), transforming '%s'\n", src);
  len = strlen(src);
  i = z_code_transform(enc, src, len);
  printf("Encoded '%d' bytes, closing encoder\n", i);
  z_code_finish(enc);
  get_and_dump(enc, 4, "aW5n", 4);
  get_and_dump(enc, 4, "eW9t", 4);
  get_and_dump(enc, 4, "Ymlu", 4);
  get_and_dump(enc, 4, "Z3lv", 4);
  get_and_dump(enc, 4, "bQ==", 4);
  get_and_dump(enc, 4, "\r\n", 2);

  src = "aW5neW9tYmluZ3lvbQ==";
  printf("\nTesting partial read (decoder side), transforming '%s'\n", src);
  len = strlen(src);
  i = z_code_transform(dec, src, len);
  printf("Decoded '%d' bytes, closing decoder\n", i);
  z_code_finish(dec);
  get_and_dump(dec, 4, "ingy", 4);
  get_and_dump(dec, 4, "ombi", 4);
  get_and_dump(dec, 4, "ngyo", 4);
  get_and_dump(dec, 4, "m", 1);

  /***********************************************************************/
  /* unget */
  src = "ihajcsuhaj";
  printf("\nTesting the unget feature, transforming '%s'\n", src);
  len = strlen(src);
  z_code_unget_result(enc, src, len);
  printf("Pushed back '%d' bytes\n", len);
  get_and_dump(enc, 1024, "ihajcsuhaj", 10);
  

  /***********************************************************************/
  /* excess whitespaces */
  src = "aW5neW9tYm\r\n   \tlu\t\rZ3\n  lvbQ==";
  printf("\nTesting the decoder with excess whitespaces, transforming '%s'\n", src);
  len = strlen(src);
  i = z_code_transform(dec, src, len);
  printf("Decoded '%d' bytes, closing decoder\n", i);
  z_code_finish(dec);
  get_and_dump(dec, 1024, "ingyombingyom", 13);

  /***********************************************************************/
  /* invalid input */
  src = "aW5neW9tYm\xffluZ3lvbQ==";
  printf("\nTesting the decoder with invalid input, transforming '%s'\n", src);
  len = strlen(src);
  i = z_code_transform(dec, src, len);
  printf("Decoded '%d' bytes, closing decoder\n", i);
  z_code_finish(dec);
  get_and_dump(dec, 1024, NULL, -1);
  if (z_code_get_errors(dec) == 0)
      return error("Errors weren't recognised");
  else
      printf("Errors recognised\n");
  z_code_clear_errors(dec);

  /***********************************************************************/
  /* invalid input - error tolerant */
  src = "aW5neW9tYm\xffluZ3lvbQ==";
  printf("\nTesting the decoder with invalid input, transforming '%s'\n", src);
  len = strlen(src);
  i = z_code_transform(dec_noerr, src, len);
  printf("Decoded '%d' bytes, closing decoder\n", i);
  z_code_finish(dec_noerr);
  get_and_dump(dec_noerr, 1024, "ingyombingyom", 13);
  if (z_code_get_errors(dec_noerr) != 0)
      return error("Errors weren't ignored");
  else
      printf("Errors ignored\n");
  z_code_clear_errors(dec_noerr);

  /***********************************************************************/
  /* huge block */
#define BLOCKSIZE (1024*1024*2)
  printf("\nTesting with a pattern block, encoding %d kbytes\n", BLOCKSIZE/1024);
  for (i = 0; i < BLOCKSIZE; i++)
    {
      if (!(i & 0x3ff))
        {
          printf("%8d\r", i);
          fflush(stdout);
        }
      q = (i & 0xff); 
      if (!z_code_transform(enc, &q, 1))
          return error("\nerror in encoding");
    }
  if (!z_code_finish(enc))
    return error("\nerror in finishing");

  len = z_code_get_result_length(enc);
  src = (char*)malloc(len);
  i = z_code_get_result(enc, src, len);
  if (i != len)
    return error("Error getting the result");

  printf("Decoding the result (%d, %d)\n", i, len);
  if (!z_code_transform(dec, src, len))
    return error("Error decoding the result");
  len = z_code_get_result_length(dec);
  if (len != BLOCKSIZE)
      return error("Result length doesn't match");
  i = z_code_get_result(dec, src, len);
  if (i != len)
      return error("Can't decode the result");
  printf("Checking the pattern in the result\n");
  for (i = 0; i < len; i++)
    {
      if (!(i & 0x3ff))
        {
          printf("%8d\r", i);
          fflush(stdout);
        }
      if (src[i] != (i & 0xff))
        return error("Decoded pattern doesn't match");
    }
  printf("Done.                    \n");

  /***********************************************************************/
  /* checks for error patterns */
  printf("\nTesting with a error patterns\n");
  if (check_for_error(dec, "AAAA", 4, FALSE)) return 1;
  if (check_for_error(dec, "AAA=", 4, FALSE)) return 1;
  if (check_for_error(dec, "AA==", 4, FALSE)) return 1;
  if (check_for_error(dec, "AA=A", 4, TRUE )) return 1;
  if (check_for_error(dec, "A===", 4, TRUE )) return 1;
  if (check_for_error(dec, "A=AA", 4, TRUE )) return 1;
  if (check_for_error(dec, "====", 4, TRUE )) return 1;
  if (check_for_error(dec, "=AAA", 4, TRUE )) return 1;
  if (check_for_error(dec, "AAA",  3, TRUE )) return 1;
  if (check_for_error(dec, "A!AA", 4, TRUE )) return 1;

  
  printf("\nDropping en/decoder\n");
  z_code_free(enc);
  z_code_free(dec);

  printf("All tests succeeded\n");
  return 0;
}
