#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define NUM_LINES 4
#define MAX_BUF_LEN 4000
#define MAX_OPS_LEN 1000
#define MAX_CHAR_LEN 3736

int main() {
  FILE *fp;
  char buffer[MAX_BUF_LEN];
  int nums[NUM_LINES][MAX_CHAR_LEN];
  char ops[MAX_OPS_LEN];
  char *bufPtr;
  int offset = 0;
  int digitsPerOp[MAX_OPS_LEN];
  uint64_t total = 0;

  fp = fopen("ex6.input", "r");

  // Grab operands
  for (int i = 0; i < NUM_LINES; i++) {
    fgets(buffer, MAX_BUF_LEN, fp);
    for (int j = 0; j < MAX_CHAR_LEN; j++) {
      if (buffer[j] > '0' && buffer[j] <= '9') {
        nums[i][j] = buffer[j] - '0';
      } else {
        nums[i][j] = 0;
      }
    }
  }

  // Grap operators
  fgets(buffer, MAX_BUF_LEN, fp);
  bufPtr = buffer;
  for (int i = 0; i < MAX_OPS_LEN; i++) {
    sscanf(bufPtr, "%c %n", &ops[i], &offset);
    bufPtr += offset;
    digitsPerOp[i] = offset - 1;
  }

  int blockIndex = 0;
  uint64_t acc;
  int currOpIndex = 0;
  int currNum = 0;
  while (blockIndex < MAX_CHAR_LEN) {
    char currOp = ops[currOpIndex];
    int digitsThisOp = digitsPerOp[currOpIndex];

    // Set base accumulator
    if (currOp == '+') {
      acc = 0;
    } else if (currOp == '*') {
      acc = 1;
    }

    // Turn each column into a number and accumulate
    for (int i = 0; i < digitsThisOp; i++) {
      for (int j = 0; j < NUM_LINES; j++) {
        int currDigit = nums[j][blockIndex + i];
        if (currDigit == 0) {
          continue;
        } else {
          currNum = currNum * 10 + currDigit;
        }
      }
      if (currOp == '+') {
        acc += currNum;
      } else {
        acc *= (uint64_t)currNum;
      }
      currNum = 0;
    }
    blockIndex += digitsThisOp + 1;
    currOpIndex++;
    total += acc;
  }

  printf("Total: %" PRIu64 "\n", total);

  fclose(fp);
}
