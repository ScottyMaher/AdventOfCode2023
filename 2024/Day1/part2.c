#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

#define TABLE_SIZE 2003 // Use a prime number to reduce clustering

typedef struct {
  int32_t key;
  int32_t value;
  bool in_use; // Flag to indicate if the slot is occupied.
} HashEntry;

// Statically allocated hash table.
static HashEntry hashTable[TABLE_SIZE];

// A simple hash function that computes an index for a given key.
uint32_t hash(int32_t key) { return ((uint32_t)key) % TABLE_SIZE; }

// Insert a key-value pair into the hash table.
// Returns true on success, false if the table is full.
bool insert(int32_t key, int32_t value) {
  uint32_t index = hash(key);
  uint32_t start = index; // Remember the starting point.

  do {
    // If the slot is empty, insert the key-value pair.
    if (!hashTable[index].in_use) {
      hashTable[index].key = key;
      hashTable[index].value = value;
      hashTable[index].in_use = true;
      return true;
    }
    // If the key already exists, update its value.
    if (hashTable[index].in_use && hashTable[index].key == key) {
      hashTable[index].value = value;
      return true;
    }
    // Move to the next slot (linear probing).
    index = (index + 1) % TABLE_SIZE;
  } while (index != start);

  // The table is full.
  return false;
}

// Look up a key in the hash table.
// If found, stores the corresponding value in *value and returns true.
bool lookup(int32_t key, int32_t *value) {
  uint32_t index = hash(key);
  uint32_t start = index;

  do {
    // If the slot is in use and the keys match, return the value.
    if (hashTable[index].in_use && hashTable[index].key == key) {
      *value = hashTable[index].value;
      return true;
    }
    // If an unused slot is encountered, the key is not present.
    if (!hashTable[index].in_use) {
      return false;
    }
    index = (index + 1) % TABLE_SIZE;
  } while (index != start);

  return false;
}

// Delete a key from the hash table.
// Returns true if the key was found and deleted, false otherwise.
bool delete_key(int32_t key) {
  uint32_t index = hash(key);
  uint32_t start = index;

  do {
    if (hashTable[index].in_use && hashTable[index].key == key) {
      // Mark the slot as unused.
      hashTable[index].in_use = false;

      // Rehash subsequent entries in the cluster to maintain proper lookup.
      uint32_t next = (index + 1) % TABLE_SIZE;
      while (hashTable[next].in_use) {
        int32_t rehashKey = hashTable[next].key;
        int32_t rehashValue = hashTable[next].value;
        hashTable[next].in_use = false; // Clear the slot.
        insert(rehashKey, rehashValue); // Reinsert the entry.
        next = (next + 1) % TABLE_SIZE;
      }
      return true;
    }
    if (!hashTable[index].in_use) {
      // An empty slot means the key is not in the table.
      return false;
    }
    index = (index + 1) % TABLE_SIZE;
  } while (index != start);

  return false;
}

int32_t main(void) {
  int32_t left[MAX_PAIRS], right[MAX_PAIRS];
  uint32_t count = 0;

  while (scanf("%u   %u\n", &left[count], &right[count]) == 2) {
    count++;
    if (count >= MAX_PAIRS) {
      fprintf(stderr, "Exceeded maximum allowed pairs.\n");
      exit(EXIT_FAILURE);
    }
  }

  clock_t start = clock();

  // initialise hashtable keys as all elements in "left", with a value of 0
  int32_t initial_value = 0;
  for (uint32_t i = 0; i < count; i++) {
    int32_t number = left[i];
    insert(number, initial_value);
  }

  // iterate through the right array and increment the value of the key
  // in the hashtable if it exists
  for (uint32_t i = 0; i < count; i++) {
    int32_t number = right[i];
    int32_t value;
    if (lookup(number, &value)) {
      insert(number, ++value);
    }
  }

  // iterate through the hashtable and sum each elements key multiplied
  // by its value
  int64_t totalDistance = 0;
  for (uint32_t i = 0; i < TABLE_SIZE; i++) {
    if (!hashTable[i].in_use) {
      continue;
    }
    int32_t number = hashTable[i].key;
    int32_t value = hashTable[i].value;
    totalDistance += (int64_t)number * (int64_t)value;
  }

  // Output the total distance.
  printf("Total distance: %lld\n", totalDistance);

  clock_t end = clock();
  double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("CPU time used: %f seconds\n", cpu_time_used);

  return 0;
}