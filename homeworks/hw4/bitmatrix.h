#include <stdlib.h>

typedef struct {
    size_t len;
    char* data;
} bitmatrix_t;

void bitmatrix_init(bitmatrix_t*, size_t);

char bitmatrix_get(bitmatrix_t*, size_t i, size_t j);
void bitmatrix_set(bitmatrix_t*, size_t i, size_t j, char val);

void bitmatrix_destroy(bitmatrix_t*);
