#include <assert.h>
#include <stdio.h>

#include "kv.h"

int main(void) {
  kv_t *table = kv_init(1024);
  assert(table != NULL);
  printf("%p\n", (void *)table);
  printf("%lu\n", table->capacity);
  printf("%lu\n", table->count);
  int idx = kv_put(table, "hehe", "haha");
  printf("%d\n", idx);
  for (size_t i = 0; i < table->capacity; i++) {
    kv_entry_t entry = table->entries[i];
    if (entry.key != NULL && entry.key != TOMBSTONE) {
      printf("[%lu] %s: %s\n", i, entry.key, entry.value);
    }
  }
  char *haha = kv_get(table, "hehe");
  char *none = kv_get(table, "none");
  assert(haha != NULL);
  assert(none == NULL);
  printf("%lu\n", table->count);

  kv_free(table);
  table = NULL;
}
