#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 20
#define REGEX1                                                                 \
  "(..).*\\1" /* Any two character sequence that appears twice, i.e. xynnxy */
#define REGEX2                                                                 \
  "(.).\\1" /* Any character that repeats once sequentially i.e. xx or yy */
int main() {
  FILE *fp = fopen("ex5.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  int totalGood = 0;

  // regexec() does NOT support lookahead assertions in POSIX
  // must separate into two regexes
  regex_t regex1, regex2;
  regcomp(&regex1, REGEX1, REG_EXTENDED);
  regcomp(&regex2, REGEX2, REG_EXTENDED);
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    if (!regexec(&regex1, buffer, 0, NULL, 0) &&
        !regexec(&regex2, buffer, 0, NULL, 0))
      totalGood++;
  }

  printf("Total good strings: %d\n", totalGood);
  fclose(fp);
  regfree(&regex1);
  regfree(&regex2);
}
