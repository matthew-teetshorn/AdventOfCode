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
    reindeers[index++] = newReindeer;
  }
  fclose(fp);

  int maxDistance = 0;
  for (int i = 0; i < NUM_REINDEER; i++) {
    reindeer_t currReindeer = reindeers[i];
    int timeFullCycle       = currReindeer.timeFlight + currReindeer.timeRest;
    int numFullCycles       = RACE_TIME_SECS / timeFullCycle;
    int timeRemaining       = RACE_TIME_SECS % timeFullCycle;

    // Strip remaining rest time from last cycle
    if (timeRemaining >= currReindeer.timeFlight) {
      numFullCycles++;
      timeRemaining = 0;
    }

    int totalTimeFlight =
        numFullCycles * currReindeer.timeFlight + timeRemaining;
    int currDistance = totalTimeFlight * currReindeer.topSpeed;
    if (currDistance > maxDistance) maxDistance = currDistance;
  }

  printf("Winning reindeer distance: %d\n", maxDistance);
}
