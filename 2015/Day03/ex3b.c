#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE     8200
#define KEY_LEN      9 /* '(+-)NNN(+-)NNN\0' */
#define FLIP_TURN(x) ((x) ^= 1U)

typedef struct {
  int x, y, timesVisited;
} house_t;

char *createKey(int x, int y) {
  static char newkey[KEY_LEN];
  // clang-format off
  sprintf(newkey, "%+04d%+04d", x, y);
  // clang-format on
  return newkey;
}

int main() {
  FILE *fp = fopen("ex3.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char keyStore[MAX_LINE][KEY_LEN] = {0};
  house_t dataStore[MAX_LINE]      = {0};

  // Store houses in a hashmap for collisions and quick access
  hcreate(MAX_LINE);
  ENTRY e;
  char *currkey;
  house_t santahouse = {0, 0, 1};
  house_t robothouse = {0, 0, 1};
  house_t *currhouse = &santahouse;
  int keyIndex       = 1;
  int numVisited     = 1;
  char c;

  // Put starting house in the hashtable
  strcpy(keyStore[0], createKey(0, 0));
  dataStore[0]  = santahouse;
  int santaturn = 1;
  e.key         = keyStore[0];
  e.data        = &dataStore[0];
  ENTRY *eFound = hsearch(e, ENTER);
  if (!eFound) {
    printf("Error inserting first element into hashtable\n");
    return EXIT_FAILURE;
  }

  while (fscanf(fp, "%c", &c) != EOF) {
    if (santaturn) {
      currhouse = &santahouse;
    } else {
      currhouse = &robothouse;
    }
    switch (c) {
    case '^':
      currkey = createKey(currhouse->x, ++currhouse->y);
      break;
    case 'v':
      currkey = createKey(currhouse->x, --currhouse->y);
      break;
    case '<':
      currkey = createKey(--currhouse->x, currhouse->y);
      break;
    case '>':
      currkey = createKey(++currhouse->x, currhouse->y);
      break;
    default:
      continue;
    }
    e.key  = currkey;
    e.data = NULL;
    eFound = hsearch(e, FIND);
    if (eFound) {
      ((house_t *)eFound->data)->timesVisited++;
    } else {
      strcpy(keyStore[keyIndex], currkey);
      dataStore[keyIndex] = *currhouse;
      e.key               = keyStore[keyIndex];
      e.data              = &dataStore[keyIndex];
      eFound              = hsearch(e, ENTER);
      if (!eFound) {
        printf("Error: out of memory in hashtable\n");
        return EXIT_FAILURE;
      }
      keyIndex++;
      numVisited++;
    }
    FLIP_TURN(santaturn);
  }
  printf("Number of houses visited: %d\n", numVisited);
  fclose(fp);
}
