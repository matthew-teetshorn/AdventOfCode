#include <inttypes.h>
#include <regex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <wchar.h>

#define MAX_LINE_LENGTH 1000
#define SMALL_BUFF_LEN 50
#define REGEX_BAD_ID "^([0-9]+)\\1+$"

uint64_t processNums(uint64_t start, uint64_t end) {
  uint64_t total = 0;
  char buff[SMALL_BUFF_LEN];

  regex_t regex;
  int ret = regcomp(&regex, REGEX_BAD_ID, REG_EXTENDED);
  if (ret != 0) {
    printf("Error compiling regex\n");
    exit(1);
  }

  for (uint64_t i = start; i <= end; i++) {
    sprintf(buff, "%" PRIu64, i);
    ret = regexec(&regex, buff, 0, NULL, 0);
    if (ret == 0) {
      // printf("Found: %" PRIu64 "\n", i);
      total += i;
    }
  }

  return total;
}

int main() {
  FILE *fp;
  char buffer[MAX_LINE_LENGTH];
  char *token;
  uint64_t start, end, total;
  total = 0;

  fp = fopen("ex2.input", "r");
  if (fp == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  fgets(buffer, MAX_LINE_LENGTH, fp);
  token = strtok(buffer, ",");
  while (token != NULL) {
    sscanf(token, "%" SCNu64 "-%" SCNu64, &start, &end);
    // printf("start: %" PRIu64 "\tend: %" PRIu64 "\n", start, end);
    total += processNums(start, end);
    token = strtok(NULL, ",");
  }

  printf("Sum of invalid IDs: %" PRIu64 "\n", total);
  fclose(fp);
}
