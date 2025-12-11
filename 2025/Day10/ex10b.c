#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <z3.h>
#include <z3_api.h>
#include <z3_ast_containers.h>
#include <z3_optimization.h>

#define DIM 212
#define MAX_JOLT 10
#define MAX_BUTTONS 13
#define MAX_SUBSETS 8192

typedef struct {
  int numElements;
  int members[MAX_BUTTONS];
} Set;

int compSets(const void *a, const void *b) {
  Set setA = *(const Set *)a;
  Set setB = *(const Set *)b;
  if (setA.numElements < setB.numElements) {
    return -1;
  } else if (setA.numElements > setB.numElements) {
    return 1;
  }
  return 0;
}

// Generate a sorted subset array with single sets first
void calcSubsets(Set buffer[MAX_SUBSETS], int n) {
  Set newSet;
  int memIdx = 0;
  newSet.numElements = 0;
  for (int i = 1; i < (1 << n); i++) {
    for (int j = 0; j < n; j++) {
      if ((i & (1 << j)) != 0) {
        newSet.numElements++;
        newSet.members[memIdx] = j;
        memIdx++;
      }
    }
    buffer[i - 1] = newSet;
    newSet.numElements = 0;
    memIdx = 0;
  }
  qsort(buffer, (1 << n) - 1, sizeof(Set), compSets);
}

int main() {
  FILE *fp = fopen("ex10.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[DIM + 2];
  uint16_t lights = 0;
  uint16_t buttons[MAX_BUTTONS] = {0};
  uint16_t joltages[MAX_JOLT] = {0};
  uint64_t total = 0;
  int numButtons = 0;
  int numJoltages = 1;
  int maxNumButtons = 0;
  Set subsets[MAX_SUBSETS];

  // Set up the Z3 optimizer
  Z3_config cfg = Z3_mk_config();
  Z3_context ctx = Z3_mk_context(cfg);
  Z3_del_config(cfg);
  Z3_ast Z3_buttons[MAX_BUTTONS] = {0};
  char bName[3];
  int wires[MAX_BUTTONS][MAX_JOLT];
  int numWires[MAX_BUTTONS];

  Z3_optimize opt = Z3_mk_optimize(ctx);
  Z3_optimize_inc_ref(ctx, opt);

  while (fgets(buffer, DIM + 2, fp) != NULL) {
    int numLights = 0;
    int numSteps = 0;

    // read lights
    char *bufPtr = buffer + 1;

    while (*bufPtr != '\(') {
      switch (*bufPtr) {
      case '.':
        lights <<= 1;
        numLights++;
        break;
      case '#':
        lights = (lights << 1) | 1;
        numLights++;
        break;
      }
      bufPtr++;
    }

    // read buttons
    int bIdx = 0;
    while (*bufPtr != '{') {
      if (*bufPtr == '\(') {
        int wIdx = 0;
        numButtons++;
        bufPtr++;
        buttons[bIdx] = 0;
        numWires[bIdx] = 0;
        for (int i = 0; i < numLights; i++) {
          if (*bufPtr != i + 48) {
            buttons[bIdx] <<= 1;
            continue;
          }
          wires[bIdx][wIdx] = *bufPtr - '0';
          numWires[bIdx]++;
          wIdx++;

          buttons[bIdx] = (buttons[bIdx] << 1) | 1;
          bufPtr += 2;
        }
        bIdx++;
      }
      bufPtr++;
    }

    // read joltages
    int joltIdx = 0;
    joltages[joltIdx] = 0;
    while (*(++bufPtr) != '}') {
      if (*bufPtr == ',') {
        numJoltages++;
        joltIdx++;
        joltages[joltIdx] = 0;
        continue;
      }
      joltages[joltIdx] = joltages[joltIdx] * 10 + (*bufPtr - '0');
    }

    // Process button presses
    Z3_optimize_push(ctx, opt);
    Z3_sort int_sort = Z3_mk_int_sort(ctx);
    Z3_ast args[MAX_BUTTONS];
    Z3_ast zero = Z3_mk_int(ctx, 0, int_sort);

    // Create a name for each button
    // These will be used by the solver as variables
    for (int i = 0; i < numButtons; i++) {
      snprintf(bName, 3, "B%c", (char)(i + 48));
      Z3_ast btn = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, bName), int_sort);
      Z3_buttons[i] = btn;
      args[i] = btn;
      // Button value must be >= 0
      Z3_ast b_ge_zero = Z3_mk_ge(ctx, btn, zero);
      Z3_optimize_assert(ctx, opt, b_ge_zero);
    }
    // Create minimization constraint
    // B0 + B1 + ... + Bn such that all joltage equations are solved
    Z3_ast sum = Z3_mk_add(ctx, numButtons, args);
    Z3_optimize_minimize(ctx, opt, sum);

    // Create constraints on joltage equations per button
    int argIdx;
    for (int i = 0; i < numJoltages; i++) {
      argIdx = 0;
      for (int j = 0; j < numButtons; j++) {
        for (int k = 0; k < numWires[j]; k++) {
          if (wires[j][k] == i) {
            // If this button affects this joltage value
            // add it to the solver args
            args[argIdx] = Z3_buttons[j];
            argIdx++;
          }
        }
      }
      // Sum the buttons that impact this joltage value
      // each is a constraint equation for the solver
      // i.e. B0 + B3 + B5 = joltage[i]
      Z3_ast isum = Z3_mk_add(ctx, argIdx, args);
      Z3_ast jolt = Z3_mk_int(ctx, joltages[i], int_sort);
      Z3_ast eq = Z3_mk_eq(ctx, isum, jolt);
      Z3_optimize_assert(ctx, opt, eq);
    }

    // Run the solver and check result
    int passTotal;
    Z3_lbool result = Z3_optimize_check(ctx, opt, 0, NULL);
    if (result == Z3_L_TRUE) {
      // Get model
      Z3_model model = Z3_optimize_get_model(ctx, opt);
      Z3_model_inc_ref(ctx, model);
      // Model solved for all buttons
      // iterate over the results and accumulate
      for (int i = 0; i < Z3_model_get_num_consts(ctx, model); i++) {
        Z3_ast interp = Z3_model_get_const_interp(
            ctx, model, Z3_model_get_const_decl(ctx, model, i));
        int valInt;
        Z3_get_numeral_int(ctx, interp, &valInt);
        passTotal += valInt;
      }

      Z3_model_dec_ref(ctx, model);
    }

    // Cleanup this iteration's context
    Z3_optimize_pop(ctx, opt);

    total += passTotal;
    passTotal = 0;
    numButtons = 0;
    numJoltages = 1;
    lights = 0;
  }
  printf("Total button presses: %" PRIu64 "\n", total);

  Z3_optimize_dec_ref(ctx, opt);
  Z3_del_context(ctx);
  fclose(fp);
}
