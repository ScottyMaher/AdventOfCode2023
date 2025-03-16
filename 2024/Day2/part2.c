#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ITERATIONS 100000
#define MIN_SAFE_DIFF 1
#define MAX_SAFE_DIFF 3
#define UNINITIALISED 255

typedef enum Level_Direction { NONE, INCREASING, DECREASING } Directionality;
typedef enum Problem_Damper {
  AVAILABLE,
  ACTIVATED,
  USED,
  FAILED
} ProblemDamper;

bool discrepancyEncounteredOld(uint16_t prevPrev, uint16_t prev,
                               uint16_t curr) {
  uint16_t diff = 0;
  if (curr < prev) {
    diff = prev - curr;
    // check diff first since it is always relevant, prevPrev only relevant if
    // initialised
    if (diff < MIN_SAFE_DIFF || diff > MAX_SAFE_DIFF) {
      return true;
    }
    // checking prevPrev initialised after directionality reduces comparisons
    if (prevPrev < prev && prevPrev != UNINITIALISED)
      return true;
  } else if (curr > prev) {
    diff = curr - prev;
    if (diff < MIN_SAFE_DIFF || diff > MAX_SAFE_DIFF) {
      return true;
    }
    if (prevPrev > prev && prevPrev != UNINITIALISED)
      return true;
  }
  // prev == curr case handled by diff being initialised to 0
  return diff < MIN_SAFE_DIFF || diff > MAX_SAFE_DIFF;
}

bool discrepancyEncountered(uint16_t prevPrev, uint16_t prev, uint16_t curr) {
  int16_t diff = (int16_t)curr - (int16_t)prev;
  int16_t prevDiff = (int16_t)prev - (int16_t)prevPrev;

  // Compute absolute differences without a branch
  uint16_t absDiff = (uint16_t)((diff ^ (diff >> 15)) - (diff >> 15));
  uint16_t absPrevDiff =
      (uint16_t)((prevDiff ^ (prevDiff >> 15)) - (prevDiff >> 15));

  // Discrepancy if the differences exceeds the limits or the sign of
  // the current and previous differences don't match
  return (absDiff < MIN_SAFE_DIFF) || (absDiff > MAX_SAFE_DIFF) ||
         (((absPrevDiff < MIN_SAFE_DIFF) || (absPrevDiff > MAX_SAFE_DIFF) ||
           (diff >> 15 != prevDiff >> 15)) &&
          (prevPrev != UNINITIALISED));
}

int main(void) {
  clock_t start = clock();

  Directionality establishedDirection = NONE;
  ProblemDamper problem_damper = AVAILABLE;
  uint32_t count = 0;
  uint32_t iterations = 0;
  uint16_t prevPrev = UNINITIALISED;
  uint16_t prev = UNINITIALISED;
  uint16_t curr = UNINITIALISED;
  // uint16_t arr[3] = {255, 255, 255};
  // uint16_t currentRow[16] = {0};
  uint16_t tempStore = UNINITIALISED;
  uint16_t tempStore2 = UNINITIALISED;
  char c;
  uint32_t count1 = 0;
  uint32_t count2 = 0;
  uint32_t count3 = 0;
  uint32_t count4 = 0;
  uint32_t count5 = 0;

  while (scanf("%" SCNu16, &curr) == 1) {
    if (iterations++ > MAX_ITERATIONS) {
      fprintf(stderr, "Too many iterations.\n");
      exit(EXIT_FAILURE);
    }

    // as soon as you encounter a discrepancy (based on the current triple),
    // the algorithm is as such:
    // 1. Check if the problem_damper is available. If it is, activate it.
    // 2. You have current -2, current -1, current and need current +1.
    // if current +1 doesn't exist, then the row is good.
    // 3. We know c-2, c-1, c are a bad triple, so try c-2, c-1 and c+1.
    // 4. If c-2, c-1, c+1 are a bad triple, try c-2, c, c+1.
    // 5. If c-2, c, c+1 are a bad triple, try c-1, c, c+1. We're shifting the
    // window if this fails, the row is bad. else if the problem dampener is
    // activated, continue to next line.

    // initialise the row so that you have the first three values
    if (prev == UNINITIALISED) {
      prev = curr;
      continue;
    }

    // 1. if not prev, initialise, skip to next iteration
    // 2. if prev and not prevPrev, we want to check limit discrepancy, but not
    // established direction

    if (discrepancyEncountered(prevPrev, prev, curr)) {
      if (problem_damper == AVAILABLE) {
        // 1. Check if the problem_damper is available. If it is, activate it.
        problem_damper = ACTIVATED;
        tempStore = curr;
        // if current +1 doesn't exist, then the row is good since only the
        // final element is bad
        c = getchar();
        if (c == '\n' || c == EOF) {
          count1++;
          count++;
          prevPrev = UNINITIALISED;
          prev = UNINITIALISED;
          establishedDirection = NONE;
          problem_damper = AVAILABLE;
          continue;
        }
        // 2. You have current -2, current -1, current and need current +1.
        uint16_t success = scanf("%" SCNu16, &curr) == 1;
        if (!success) {
          fprintf(stderr, "Invalid input.\n");
          exit(EXIT_FAILURE);
        }
        c = getchar();
        // 3. We know c-2, c-1, c are a bad triple, so try c-2, c-1 and c+1.
        if (!discrepancyEncountered(prevPrev, prev, curr) &&
            (establishedDirection == NONE ||
             (prev > curr && establishedDirection == DECREASING) ||
             (prev < curr && establishedDirection == INCREASING))) {
          // we wanted to check this because otherwise it would just go scanning
          // the next line
          if (c == '\n' || c == EOF) {
            count2++;
            count++;
            prevPrev = UNINITIALISED;
            prev = UNINITIALISED;
            establishedDirection = NONE;
            problem_damper = AVAILABLE;
            continue;
          } else {
            problem_damper = USED;
            prevPrev = prev;
            prev = curr;
            continue;
          }
        }
        // 4. If c-2, c-1, c+1 are a bad triple, try c-2, c, c+1.
        tempStore2 = prev;
        prev = tempStore;
        if (!discrepancyEncountered(prevPrev, prev, curr) &&
            (establishedDirection == NONE ||
             (prev > curr && establishedDirection == DECREASING) ||
             (prev < curr && establishedDirection == INCREASING))) {
          if (c == '\n' || c == EOF) {
            count3++;
            count++;
            prevPrev = UNINITIALISED;
            prev = UNINITIALISED;
            establishedDirection = NONE;
            problem_damper = AVAILABLE;
            continue;
          } else {
            problem_damper = USED;
            prevPrev = prev;
            prev = curr;
            continue;
          }
        }
        // 5. If c-2, c, c+1 are a bad triple, try c-1, c, c+1.
        prevPrev = tempStore2;
        if (!discrepancyEncountered(prevPrev, prev, curr) &&
            (establishedDirection == NONE ||
             (prev > curr && establishedDirection == DECREASING) ||
             (prev < curr && establishedDirection == INCREASING))) {
          if (c == '\n' || c == EOF) {
            count4++;
            count++;
            prevPrev = UNINITIALISED;
            prev = UNINITIALISED;
            establishedDirection = NONE;
            problem_damper = AVAILABLE;
            continue;
          } else {
            problem_damper = USED;
            prevPrev = prev;
            prev = curr;
          }

          // check that the direction matches the established direction
          if (establishedDirection == NONE) {
            if (prev > curr) {
              establishedDirection = DECREASING;
            } else if (prev < curr) {
              establishedDirection = INCREASING;
            }
          } else if (establishedDirection == INCREASING && prev > curr) {
            // reset and skip to the next line
            prevPrev = UNINITIALISED;
            prev = UNINITIALISED;
            establishedDirection = NONE;
            problem_damper = AVAILABLE;
            while (c != '\n' && c != EOF) {
              c = getchar();
              continue;
            }
            continue;
          } else if (establishedDirection == DECREASING && prev < curr) {
            // reset and skip to the next line
            prevPrev = UNINITIALISED;
            prev = UNINITIALISED;
            establishedDirection = NONE;
            problem_damper = AVAILABLE;
            while (c != '\n' && c != EOF) {
              c = getchar();
              continue;
            }
            continue;
          }
          continue;
        }
        // if we reach this point, the row is bad, reset and skip to next line
        prevPrev = UNINITIALISED;
        prev = UNINITIALISED;
        establishedDirection = NONE;
        problem_damper = AVAILABLE;
        while (c != '\n' && c != EOF) {
          c = getchar();
          continue;
        }
        continue;

      } else {
        // reset and skip to the next line
        prevPrev = UNINITIALISED;
        prev = UNINITIALISED;
        establishedDirection = NONE;
        problem_damper = AVAILABLE;
        while (c != '\n' && c != EOF) {
          c = getchar();
          continue;
        }
        continue;
      }
    }

    if (establishedDirection == NONE && prevPrev != UNINITIALISED) {
      if (prev > curr) {
        establishedDirection = DECREASING;
      } else if (prev < curr) {
        establishedDirection = INCREASING;
      }
    }

    // if we reached the end of line '\n' or EOF and haven't continued, its safe
    c = getchar();
    if (c == '\n' || c == EOF) {
      count5++;
      count++;
      prevPrev = UNINITIALISED;
      prev = UNINITIALISED;
      establishedDirection = NONE;
      problem_damper = AVAILABLE;
    } else if (problem_damper == ACTIVATED) {
      continue;
    } else {
      prevPrev = prev;
      prev = curr;
    }
  }

  clock_t end = clock();
  double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("CPU time used: %f seconds\n", cpu_time_used);
  printf("%" PRIu32 " safe reports\n", count);
  printf("count1: %" PRIu32 "\n", count1);
  printf("count2: %" PRIu32 "\n", count2);
  printf("count3: %" PRIu32 "\n", count3);
  printf("count4: %" PRIu32 "\n", count4);
  printf("count5: %" PRIu32 "\n", count5);

  return 0;
}