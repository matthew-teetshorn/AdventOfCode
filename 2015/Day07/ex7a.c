#include <inttypes.h>
#include <search.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE     20
#define NUM_REGS     339
#define LEN_REG_NAME 4
#define DEBUG        0

typedef enum { LDI, LDR, OR, ANDI, ANDR, LSHIFT, RSHIFT, NOT } opcode_t;
typedef enum { INSERT, LOOKUP, FREE } regQuery_t;

typedef union {
  uint16_t ival;
  char cval[LEN_REG_NAME];
} input_t;

typedef struct {
  input_t lhs;
  input_t rhs;
  opcode_t opcode;
  uint16_t value;
  uint16_t valueGood;
} reg_t;

reg_t *regQuery(char *regName, regQuery_t query, reg_t *regContent) {
  static char *regKeys[NUM_REGS];
  static reg_t *regData[NUM_REGS];
  static int regIndex = 0;

  ENTRY e;
  ENTRY *ep;
  e.key  = regName;
  e.data = NULL;

  if (query == INSERT) {
    regKeys[regIndex] = malloc(LEN_REG_NAME);
    if (regKeys[regIndex] == NULL) return NULL;

    regData[regIndex] = malloc(sizeof(reg_t));
    if (regData[regIndex] == NULL) {
      free(regKeys[regIndex]);
      return NULL;
    }

    strcpy(regKeys[regIndex], regName);
    *regData[regIndex] = *regContent;
    e.key              = regKeys[regIndex];
    e.data             = regData[regIndex];

    ep = hsearch(e, ENTER);
    regIndex++;

    return ep->data;

  } else if (query == LOOKUP) {
    ep = hsearch(e, FIND);
    if (ep != NULL) return ep->data;

  } else if (query == FREE) {
    for (int i = 0; i < regIndex; i++) {
      free(regKeys[i]);
      free(regData[i]);
    }
  }

  reg_t *retval;
  return retval;
}

uint16_t evalReg(reg_t *reg) {
  if (reg->valueGood) return reg->value;

  uint16_t retval = 0;
  uint16_t lresult, rresult;
  char *lname, *rname;
  switch (reg->opcode) {
  case LDI:
    reg->value = reg->lhs.ival;
    if (DEBUG) printf("Result of LDI %d: %d\n", reg->lhs.ival, reg->value);
    break;
  case LDR:
    lname      = reg->lhs.cval;
    lresult    = evalReg(regQuery(lname, LOOKUP, NULL));
    reg->value = lresult;
    if (DEBUG)
      printf("Result of LDR from %s: %d = %d\n", lname, lresult, reg->value);
    break;
  case OR:
    lname      = reg->lhs.cval;
    rname      = reg->rhs.cval;
    lresult    = evalReg(regQuery(lname, LOOKUP, NULL));
    rresult    = evalReg(regQuery(rname, LOOKUP, NULL));
    reg->value = lresult | rresult;

    if (DEBUG)
      printf("Result of OR between %s: %d and %s: %d = %d\n", lname, lresult,
             rname, rresult, reg->value);
    break;
  case ANDI:
    rname      = reg->rhs.cval;
    lresult    = reg->lhs.ival;
    rresult    = evalReg(regQuery(rname, LOOKUP, NULL));
    reg->value = lresult & rresult;

    if (DEBUG)
      printf("Result of ANDI between %d and %s: %d = %d\n", lresult, rname,
             rresult, reg->value);
    break;
  case ANDR:
    lname      = reg->lhs.cval;
    rname      = reg->rhs.cval;
    lresult    = evalReg(regQuery(lname, LOOKUP, NULL));
    rresult    = evalReg(regQuery(rname, LOOKUP, NULL));
    reg->value = lresult & rresult;

    if (DEBUG)
      printf("Result of ANDR between %s: %d and %s: %d = %d\n", lname, lresult,
             rname, rresult, reg->value);
    break;
  case LSHIFT:
    lname      = reg->lhs.cval;
    lresult    = evalReg(regQuery(lname, LOOKUP, NULL));
    rresult    = reg->rhs.ival;
    reg->value = lresult << rresult;

    if (DEBUG)
      printf("Result of LSHIFT %d and %s: %d = %d\n", rresult, lname, lresult,
             reg->value);
    break;
  case RSHIFT:
    lname      = reg->lhs.cval;
    lresult    = evalReg(regQuery(lname, LOOKUP, NULL));
    rresult    = reg->rhs.ival;
    reg->value = lresult >> rresult;

    if (DEBUG)
      printf("Result of RSHIFT %d and %s: %d = %d\n", rresult, lname, lresult,
             reg->value);
    break;
  case NOT:
    rname      = reg->rhs.cval;
    rresult    = evalReg(regQuery(rname, LOOKUP, NULL));
    reg->value = ~rresult;
    if (DEBUG)
      printf("Result of NOT on %s: %d = %d\n", rname, rresult, reg->value);
    break;
  }

  // Memoization
  reg->valueGood = 1;
  return reg->value;
}

int main() {
  FILE *fp = fopen("ex7.input", "r");
  if (fp == NULL) {
    printf("Error reading file\n");
    return EXIT_FAILURE;
  }

  hcreate(NUM_REGS); // Hashtable to store registers for lookup
  char buffer[MAX_LINE];

  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    reg_t regData     = {0};
    regData.valueGood = 0;
    char regName[LEN_REG_NAME];

    if (sscanf(buffer, "%" SCNu16 " -> %s", &regData.lhs.ival, regName) ==
        2) { // LDI: 'n -> r1'
      regData.opcode = LDI;
    } else if (sscanf(buffer, "%s -> %s", regData.lhs.cval, regName) ==
               2) { // LDR: 'r1 -> r2'
      regData.opcode = LDR;
    } else if (sscanf(buffer, "%s OR %s -> %s", regData.lhs.cval,
                      regData.rhs.cval,
                      regName) == 3) { // OR: 'r1 OR r2 -> r3'
      regData.opcode = OR;
    } else if (sscanf(buffer, "%" SCNu16 " AND %s -> %s", &regData.lhs.ival,
                      regData.rhs.cval,
                      regName) == 3) { // ANDI: 'n AND r1 -> r2'
      regData.opcode = ANDI;
    } else if (sscanf(buffer, "%s AND %s -> %s", regData.lhs.cval,
                      regData.rhs.cval,
                      regName) == 3) { // ANDR: 'r1 AND r2 -> r3'
      regData.opcode = ANDR;
    } else if (sscanf(buffer, "%s LSHIFT %" SCNu16 " -> %s", regData.lhs.cval,
                      &regData.rhs.ival,
                      regName) == 3) { // LSHIFT: 'r1 LSHIFT n -> r2'
      regData.opcode = LSHIFT;
    } else if (sscanf(buffer, "%s RSHIFT %" SCNu16 " -> %s", regData.lhs.cval,
                      &regData.rhs.ival,
                      regName) == 3) { // RSHIFT: 'r1 RSHIFT n -> r2'
      regData.opcode = RSHIFT;
    } else if (sscanf(buffer, "NOT %s -> %s", regData.rhs.cval, regName) ==
               2) { // NOT: 'NOT r1 -> r2'
      regData.opcode = NOT;
    }

    regQuery(regName, INSERT, &regData);
  }

  reg_t *eval = regQuery("a", LOOKUP, NULL);
  int final   = evalReg(eval);
  printf("Value at register a: %" SCNu16 "\n", final);

  // Cleanup
  regQuery(NULL, FREE, NULL);
  hdestroy();
  fclose(fp);
}
