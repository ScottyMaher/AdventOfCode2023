#include <stdio.h>
#include <stdlib.h>

#define PRIu8 "hhu"
#define PRId8 "hhd"
#define PRIx8 "hhx"
#define PRIu16 "hu"
#define PRId16 "hd"
#define PRIx16 "hx"
#define PRIu32 "u"
#define PRId32 "d"
#define PRIx32 "x"
#define PRIu64 "llu" // or possibly "lu"
#define PRId64 "lld" // or possibly "ld"
#define PRIx64 "llx" // or possibly "lx"

#define MAX_PAIRS 10000
#define MAX_STACK (2 * MAX_PAIRS)

// Swap two integer values.
void swap(uint32_t *a, uint32_t *b) {
  uint32_t temp = *a;
  *a = *b;
  *b = temp;
}

// Partition the subarray arr[low..high] using the last element as the pivot.
uint32_t partition(uint32_t arr[], uint32_t low, uint32_t high) {
  uint32_t pivot = arr[high]; // Choose the pivot.
  int32_t i = low - 1;        // Place for the smaller element.

  for (uint32_t j = low; j < high; j++) {
    if (arr[j] <= pivot) {
      i++;
      swap(&arr[i], &arr[j]);
    }
  }
  // Place the pivot element at the correct position.
  swap(&arr[i + 1], &arr[high]);
  return i + 1;
}

void iterativeQuickSort(uint32_t arr[], uint32_t n) {
  uint32_t stack[MAX_STACK];
  int32_t top = -1; // Stack is initially empty.

  // Push initial indices (low and high) onto the stack.
  stack[++top] = 0;
  stack[++top] = n - 1;

  // Continue until there are no more subarrays to sort.
  while (top >= 0) {
    // Pop high and low indices.
    uint32_t high = stack[top--];
    uint32_t low = stack[top--];

    // Partition the current subarray.
    uint32_t p = partition(arr, low, high);

    // If elements exist on the left side of the pivot, push left subarray
    // indices.
    if (p - 1 > low) {
      stack[++top] = low;
      stack[++top] = p - 1;
    }
    // If elements exist on the right side of the pivot, push right subarray
    // indices.
    if (p + 1 < high) {
      stack[++top] = p + 1;
      stack[++top] = high;
    }
  }
}

int main(void) {
  uint32_t left[MAX_PAIRS], right[MAX_PAIRS];
  uint32_t count = 0;

  while (scanf("%u   %u\n", &left[count], &right[count]) == 2) {
    count++;
    if (count >= MAX_PAIRS) {
      fprintf(stderr, "Exceeded maximum allowed pairs.\n");
      exit(EXIT_FAILURE);
    }
  }

  iterativeQuickSort(left, count);
  iterativeQuickSort(right, count);

  // Sum the absolute differences between corresponding elements.
  int64_t totalDistance = 0;
  for (uint32_t i = 0; i < count; i++) {
    int64_t diff = (int64_t)left[i] - (int64_t)right[i];
    if (diff < 0)
      diff = -diff;
    totalDistance += diff;
  }

  // Output the total distance.
  printf("Total distance: %llu\n", totalDistance);

  return 0;
}