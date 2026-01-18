#include <math.h>
#include <stdio.h>

#define MIN_VAL     34000000
#define NUM_FACTORS 10000

typedef struct {
  int count;
  int factors[NUM_FACTORS];
} factor_t;

factor_t calcFactors(int num) {
  int split = (int)sqrt(num) + 1;
  if (num == 0) {
    return (factor_t){0, {0}};
  } else if (num == 1) {
    return (factor_t){1, {1}};
  }

  factor_t factors;
  factors.count      = 2;
  factors.factors[0] = 1;
  factors.factors[1] = num;

  for (int i = 2; i < split; i++) {
    if (factors.count >= NUM_FACTORS) printf("Too many factors\n");
    if (num % i == 0) {
      factors.factors[factors.count++] = i;
      int quot                         = num / i;
      if (i != quot) {
        factors.factors[factors.count++] = quot;
      }
    }
  }
  return factors;
}

int main() {
  int index = 1;
  while (1) {
    factor_t factors = calcFactors(index);
    int count        = 0;
    for (int i = 0; i < factors.count; i++) {
      count += factors.factors[i] * 10;
    }
    if (count >= MIN_VAL) break;
    index++;
  }

  printf("First house with %d presents: %d\n", MIN_VAL, index);
}
