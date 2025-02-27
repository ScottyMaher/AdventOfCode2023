#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ITERATIONS 100000
#define MIN_SAFE_DIFF 1
#define MAX_SAFE_DIFF 3

typedef enum Level_Direction { NONE, INCREASING, DECREASING } Level;
typedef enum Problem_Damper { AVAILABLE, ACTIVATED, USED } ProblemDamper;

bool exceedsLevelDiff(uint16_t prev, uint16_t curr) {
  uint16_t diff = 0;
  if (prev > curr) {
    diff = prev - curr;
  } else if (curr > prev) {
    diff = curr - prev;
  }
  return diff < MIN_SAFE_DIFF || diff > MAX_SAFE_DIFF;
}

int main(void) {
  clock_t start = clock();

  Level level = NONE;
  ProblemDamper problem_damper = AVAILABLE;
  uint32_t count = 0;
  uint32_t iterations = 0;
  uint16_t prev = 255;
  uint16_t curr = 255;

  while (scanf("%" SCNu16, &curr) == 1) {
    if (iterations++ > MAX_ITERATIONS) {
      fprintf(stderr, "Too many iterations.\n");
      exit(EXIT_FAILURE);
    }

    if (prev == 255) {
      prev = curr;
      continue;
    }

    if (exceedsLevelDiff(prev, curr) || (level == INCREASING && prev > curr) ||
        (level == DECREASING && prev < curr)) {
      if (problem_damper == AVAILABLE) {
        problem_damper = ACTIVATED;
      } else {
        // reset and skip to the next line
        prev = 255;
        level = NONE;
        problem_damper = AVAILABLE;
        while (getchar() != '\n') {
          continue;
        }
        continue;
      }
    }

    if (level == NONE) {
      if (prev > curr) {
        level = DECREASING;
      } else if (prev < curr) {
        level = INCREASING;
      }
    }

    // if we reached the end of line '\n' or EOF and haven't continued, its safe
    char c = getchar();
    if (c == '\n' || c == EOF) {
      count++;
      prev = 255;
      level = NONE;
      problem_damper = AVAILABLE;
    } else if (problem_damper == ACTIVATED) {
      continue;
    } else {
      prev = curr;
    }
  }

  clock_t end = clock();
  double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("CPU time used: %f seconds\n", cpu_time_used);
  printf("%" PRIu32 " safe reports\n", count);

  return 0;
}