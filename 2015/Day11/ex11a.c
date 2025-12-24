#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE    20
#define NUM_TO_FIND 2

bool incBuffer(char *buffer) {
  int len = strlen(buffer);
  for (int i = len - 1; i >= 0; i--) {
    if (buffer[i] == 'z') {
      buffer[i] = 'a';
      if (i == 0) return false;
    } else {
      buffer[i] += 1;
      return true;
    }
  }
  return false;
}

bool pwIsValid(char *pw) {
  /* Check for straight of len 3 */
  char *p = pw;
  int len = strlen(pw);
  while (p - pw < len - 2) {
    if (*(p + 1) - *p == 1 && *(p + 2) - *p == 2) {
      break;
    }
    p++;
  }
  if (p - pw >= len - 2) return false;

  /* Check for disallowed letters */
  p = pw;
  while (p - pw < len) {
    if (*p == 'i' || *p == 'l' || *p == 'o') return false;
    p++;
  }

  /* Check for 2 non-overlapping pairs */
  int pairsFound = 0;
  p              = pw;
  while (p - pw < len - 1) {
    if (*p == *(p + 1)) {
      p++;
      pairsFound++;
    }
    p++;
  }

  if (pairsFound >= 2) return true;
  return false;
}

int main() {
  FILE *fp = fopen("ex11.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  fgets(buffer, MAX_LINE, fp);
  fclose(fp);

  buffer[strlen(buffer) - 1] = '\0';
  int numFound               = 0;

  while (incBuffer(buffer)) {
    if (pwIsValid(buffer)) {
      numFound++;
      printf("Found pw: %s\n", buffer);
      if (numFound == NUM_TO_FIND) break;
    }
  }

  if (numFound == 0) printf("New valid pw not found\n");
}
