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

#define HASHSIZE 1000

struct nlist {        /* table entry: */
  struct nlist *next; /* next entry in chain */
  uint32_t *number;   /* defined name */
  uint16_t *count;    /* replacement text */
};

static struct nlist *hashtable[HASHSIZE]; /* pointer table */

/* hash: form hash value for number */
uint32_t hash(uint32_t *number) { return *number % HASHSIZE; }

/* lookup: look for number in hashtable */
struct nlist *lookup(uint32_t *number) {
  struct nlist *np;
  uint32_t hashval = hash(number);
  for (np = hashtable[hashval]; np != NULL; np = np->next)
    if (number == np->number)
      return np; /* found */
  return NULL;   /* not found */
}

/* install: put (name, defn) in hashtab */
struct nlist *install(uint32_t *number, uint16_t *count) {
  struct nlist *np;
  uint32_t hashval;
  if ((np = lookup(number)) == NULL) { /* not found */
    np = (struct nlist *)malloc(sizeof(*np));
    if (np == NULL || (np->number = number) == NULL)
      return NULL;
    hashval = hash(number);
    np->next = hashtable[hashval];
    hashtable[hashval] = np;
  } else                     /* already there */
    free((void *)np->count); /*free previous defn */
  if ((np->count = count) == NULL)
    return NULL;
  return np;
}