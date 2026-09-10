#include <stdio.h>

#include "kv.h"

int main(void) {
  kv_t *table = kv_init(3);
  printf("%p\n", (void *)table);

  if (table != NULL) {
    printf("%lu\n", table->capacity);
  }
}
