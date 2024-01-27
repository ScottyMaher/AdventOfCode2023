#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512
#define NUM_SEEDS 20

typedef enum {
  NONE,
  SEEDS,
  SEED_TO_SOIL,
  SOIL_TO_FERTILIZER,
  FERTILIZER_TO_WATER,
  WATER_TO_LIGHT,
  LIGHT_TO_TEMPERATURE,
  TEMPERATURE_TO_HUMIDITY,
  HUMIDITY_TO_LOCATION,
} Section;

uint32_t seeds[NUM_SEEDS];

int8_t read_file() {
  FILE *file = fopen("day5.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file)) {

    char* tokenSavePrt = NULL;
    char* token = strtok_r(line, " \n", &tokenSavePrt);

    uint8_t index = 0;
    while(token != NULL && index < NUM_SEEDS) {
      seeds[index] = atoi(token);
      index++;
      token = strtok_r(NULL, " ", &tokenSavePrt);
    }

    for (uint8_t i = 0; i < NUM_SEEDS; i++) {
      printf("%u\n", seeds[i]);
    }

    // char* test1 = strstr(line, "seeds:");

    break;
  }

  fclose(file);
  return 0;
}



int main() {

  read_file();
  printf("Gay :)");
  return 0;
}