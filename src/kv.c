#include "kv.h"

static size_t hash(__attribute__((nonnull)) const char *val, size_t capacity);

kv_t *kv_init(size_t capacity) {
  if (capacity == 0)
    return NULL;

  kv_t *table = (kv_t *)malloc(sizeof(kv_t));
  if (table == NULL)
    return NULL;

  table->capacity = capacity;
  table->count = 0;

  table->entries = (kv_entry_t *)calloc(sizeof(kv_entry_t), capacity);
  if (table->entries == NULL) {
    free(table);
    return NULL;
  }

  return table;
}

/*
 * Insert a key-value pair into the db.
 *
 * Returns 0 if successful,
 * -1 on invalid arguments, or -2 when the db is full.
 */
int kv_put(kv_t *db, char *key, char *value) {
  if (db == NULL || key == NULL || value == NULL)
    return -1;

  size_t idx = hash(key, db->capacity);

  for (size_t i = 0; i < db->capacity; i++) {
    size_t real_idx = (idx + i) % db->capacity;
    kv_entry_t *entry = &db->entries[real_idx];

    if (entry->key == NULL || entry->key == TOMBSTONE) {
      char *newkey = strdup(key);
      char *newval = strdup(value);
      if (newkey == NULL || newval == NULL) {
        free(newkey);
        free(newval);
        return -1;
      }
      entry->key = newkey;
      entry->value = newval;
      db->count++;
      return 0;
    } else if (strcmp(entry->key, key) == 0) {
      char *newval = strdup(value);
      if (newval == NULL) {
        return -1;
      }
      free(entry->value);
      entry->value = newval;
      return 0;
    }
  }

  return -2;
}

char *kv_get(kv_t *db, char *key) {
  if (db == NULL || key == NULL)
    return NULL;

  size_t idx = hash(key, db->capacity);

  for (size_t i = 0; i < db->capacity - 1; i++) {
    size_t real_idx = (idx + i) % db->capacity;
    kv_entry_t entry = db->entries[real_idx];
    if (entry.key == NULL) {
      break;
    } else if (entry.key == TOMBSTONE) {
      continue;
    } else if (strcmp(entry.key, key) == 0) {
      return entry.value;
    }
  }
  return NULL;
}

void kv_free(kv_t *db) {
  if (db == NULL)
    return;

  free(db->entries);
  free(db);
}

static size_t hash(__attribute__((nonnull)) const char *value,
                   size_t capacity) {
  size_t h = 0x13371337deadbeef;
  while (*value) {
    h ^= (size_t)*value;
    h <<= 8;
    h += (size_t)*value;
    value++;
  }
  return h % capacity;
}
