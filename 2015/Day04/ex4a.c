#include <openssl/evp.h>
#include <openssl/opensslconf.h>
#include <pthread.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char input[] = "iwrupvqb";
#define INPUT_LEN   (sizeof(input) / sizeof(input[0]))
#define MD5_HEX_LEN 32
#define NUM_LEN     8 /* 10000000 */
#define REGEX_P1    "^0{5}"
#define REGEX_P2    "^0{6}"

int main() {
  char numstr[NUM_LEN + 1] = {0};
  char hashinput[INPUT_LEN + NUM_LEN];

  unsigned int md_len;
  unsigned char md_value[EVP_MAX_MD_SIZE];
  char md5_hash[MD5_HEX_LEN + 1] = "";
  EVP_MD_CTX *mdctx;

  regex_t regex;
  const char *pattern = REGEX_P2;
  if (regcomp(&regex, pattern, REG_EXTENDED)) {
    printf("Error compiling regex\n");
    return EXIT_FAILURE;
  }

  // Create the MD5 context
  mdctx = EVP_MD_CTX_new();
  if (mdctx == NULL) {
    printf("Error creating MD5 context\n");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < 10000000; i++) {
    sprintf(numstr, "%d", i);
    strcpy(hashinput, input);
    strcat(hashinput, numstr);

    // reset the context for a new hash
    if (1 != EVP_DigestInit_ex(mdctx, EVP_md5(), NULL)) {
      printf("Error initializing EVP_Digest\n");
      EVP_MD_CTX_free(mdctx);
      return EXIT_FAILURE;
    }

    // Place the hashinput into the context digest
    if (1 != EVP_DigestUpdate(mdctx, hashinput, strlen(hashinput))) {
      printf("Error updated the EVP_Digest\n");
      EVP_MD_CTX_free(mdctx);
      return EXIT_FAILURE;
    }

    // Calculate the MD5 hash result
    if (1 != EVP_DigestFinal_ex(mdctx, md_value, &md_len)) {
      printf("Error on EVP_DigestFinal_ex\n");
      EVP_MD_CTX_free(mdctx);
      return EXIT_FAILURE;
    }

    // Print the md5 hash from bytes to hex
    for (int j = 0; j < md_len; j++) {
      sprintf(md5_hash + (2 * j), "%02x", md_value[j]);
    }

    // Look for the 5 leading zeroes for termination
    if (!regexec(&regex, md5_hash, 0, NULL, 0)) {
      printf("Match found at: i = %d\n", i);
      printf("Hash value    : %s\n", md5_hash);
      break;
    }
  }

  // Cleanup
  regfree(&regex);
  EVP_MD_CTX_free(mdctx);
}
