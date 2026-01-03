#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE       76
#define MAX_NAME_LEN   7
#define NUM_REINDEER   9
#define RACE_TIME_SECS 2503

typedef struct {
  int topSpeed;
  int timeFlight;
  int timeRest;
  int currDist;
  int currScore;
  char name[MAX_NAME_LEN + 1];
} reindeer_t;

int main() {
  FILE *fp = fopen("ex14.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  reindeer_t reindeers[NUM_REINDEER];
  int index = 0;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    reindeer_t newReindeer;
    // clang-format off
    sscanf(buffer, "%s can fly %d km/s for %d seconds, but then must rest for %d seconds.",
      newReindeer.name, &newReindeer.topSpeed, &newReindeer.timeFlight, &newReindeer.timeRest);
    // clang-format on
    newReindeer.currDist  = 0;
    newReindeer.currScore = 0;
    reindeers[index++]    = newReindeer;
  }

  for (int currTime = 1; currTime <= RACE_TIME_SECS; currTime++) {
    int currMaxDistance = 0;
    for (int i = 0; i < NUM_REINDEER; i++) {
      reindeer_t *currReindeer = &reindeers[i];
      int timeFullCycle = currReindeer->timeFlight + currReindeer->timeRest;
      int timeCurrCycle = currTime % timeFullCycle;
      int isFlying =
          timeCurrCycle > 0 && timeCurrCycle <= currReindeer->timeFlight ? 1
                                                                         : 0;
      if (isFlying) {
        currReindeer->currDist += currReindeer->topSpeed;
      }
      if (currReindeer->currDist > currMaxDistance) {
        currMaxDistance = currReindeer->currDist;
      }
    }

    for (int i = 0; i < NUM_REINDEER; i++) {
      if (reindeers[i].currDist == currMaxDistance) {
        reindeers[i].currScore++;
      }
    }
  }

  int winningPoints = 0;
  for (int i = 0; i < NUM_REINDEER; i++) {
    if (reindeers[i].currScore > winningPoints)
      winningPoints = reindeers[i].currScore;
  }

  printf("Winning reindeer points: %d\n", winningPoints);
}
