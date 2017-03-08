#ifndef _MAP_H_
#define _MAP_H_

#include <stdint.h>

typedef struct {
    char* key;
    uint8_t value;
} map_pair_t;

typedef struct {
    map_pair_t* mapping;
    int size;
    int alloc_size;
} map_t;


map_t* map_init();
/**
 * Returns either `true` or `false` depending on whether the key is found. So, the value is set in the value pointer
 */
uint8_t map_get(map_t*, char* key, uint8_t* value);

/**
 * Sets the value of the mapping given a key. If the key does not already exist, it gets added to the map
 */
void map_set(map_t*, char* key, uint8_t value);

/**
 * Removes a key from the map. If one exists, it will return true. Otherwise, returns false
 */
uint8_t map_remove(map_t*, char* key);

/**
 * Delete the map from memory
 */
void map_destroy(map_t*);

#endif
