#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define NUM_LINES 4
#define MAX_OPS_LEN 1000
#define MAX_BUF_LEN 4000

int main() {
  FILE *fp;
  char buffer[MAX_BUF_LEN];
  int nums[NUM_LINES][MAX_OPS_LEN];
  char ops[MAX_OPS_LEN];
  char *bufPtr;
  int offset = 0;
  uint64_t total = 0;

  fp = fopen("ex6.input", "r");

  // Grab operands
  for (int i = 0; i < NUM_LINES; i++) {
    fgets(buffer, MAX_BUF_LEN, fp);
    bufPtr = buffer;
    for (int j = 0; j < MAX_OPS_LEN; j++) {
      sscanf(bufPtr, "%d%n", &nums[i][j], &offset);
      bufPtr += offset;
    }
  }

  // Grap operators
  fgets(buffer, MAX_BUF_LEN, fp);
  bufPtr = buffer;
  for (int i = 0; i < MAX_OPS_LEN; i++) {
    sscanf(bufPtr, "%c %n", &ops[i], &offset);
    bufPtr += offset;
  }

  for (int i = 0; i < MAX_OPS_LEN; i++) {
    if (ops[i] == '+') {
      total += nums[0][i] + nums[1][i] + nums[2][i] + nums[3][i];
    } else if (ops[i] == '*') {
      total += (uint64_t)nums[0][i] * nums[1][i] * nums[2][i] * nums[3][i];
    }
  }

  printf("Total: %" PRIu64 "\n", total);

  fclose(fp);
}
