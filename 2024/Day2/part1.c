#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ITERATIONS 100000
#define MIN_SAFE_DIFF 1
#define MAX_SAFE_DIFF 3

typedef enum Level_Direction { NONE, INCREASING, DECREASING } Level;

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
  Level level = NONE;
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
      // reset and skip to the next line
      prev = 255;
      level = NONE;
      while (getchar() != '\n') {
        continue;
      }
      continue;
    }

    if (level == NONE) {
      if (prev > curr) {
        level = DECREASING;
      } else if (prev < curr) {
        level = INCREASING;
      } else {
        fprintf(stderr, "Invalid input.\n");
        exit(EXIT_FAILURE);
      }
    }

    // if we reached the end of line '\n' or EOF and haven't continued, its safe
    char c = getchar();
    if (c == '\n' || c == EOF) {
      count++;
      prev = 255;
      level = NONE;
    } else {
      prev = curr;
    }
  }

  printf("%" PRIu32 " safe reports\n", count);

  return 0;
}