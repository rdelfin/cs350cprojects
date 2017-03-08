#include "map.h"
#include <string.h>
#include <stdlib.h>

char* copy_str(char* str) {
    char* newstr = malloc(strlen(str));
    strncpy(newstr, str, strlen(str));
}

int64_t map_search(map_t* map, char* key) {
    // Find the key
    long int idx = -1;
    for(int i = 0; i < map->size; i++) {
        if(strcmp(map->mapping[i].key, key) == 0) {
            idx = i;
            break;
        }
    }

    return idx;
}

map_t* map_init() {
    map_t* map = malloc(sizeof(map_t));
    map->size = 0;
    map->alloc_size = 1;
    map->mapping = malloc(sizeof(map_pair_t)*alloc_size);
}


uint8_t map_get(map_t* map, char* key, uint8_t* value) {
    int64_t idx = map_search(map, key);
    if(idx >= 0) {
        *value = map->mapping[idx].value;
        return 1;
    }
   
   return 0;
}

void map_set(map_t* map, char* key, uint8_t value) {
    int64_t idx = map_search(map, key);
    if(idx >= 0) {
        map->mapping[idx].value = value;
        return;
    }
    
    // If we get here, the key does not exist already
    // First, allocate more space if necessary
    if(map->size >= map->alloc_size) {
        map->alloc_size *= 2;
        map->mapping = realloc(sizeof(map_pair_t) * size);
    }

    map_pair_t pair;
    pair.key = copy_str(key);
    pair.value = value;

    map->mapping[map->size++] = pair;
}

uint8_t map_remove(map_t* map, char* key) {
    int64_t idx = map_search(map, key);

    // Remove the item by overwritting the element and reducing size
    if(idx >= 0) {
        map->size--;
        for(int i = idx; i < map->size; i++) {
            map->mapping[i] = map->mapping[i+1];
        }
    }
}
            

void map_destroy(map_t* map) {
    for(int i = 0; i < map->size; i++) {
       free(map->mapping[i].key);
    }

    free(map->mapping);
    free(map);
}

