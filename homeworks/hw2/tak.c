#include <stdio.h>
#include <stdlib.h>

int tarai(int x, int y, int z);

int main(int argc, char* argv[]) {
    if(argc != 4) {
        printf("E: You need to input exactly 3 arguments: %s x y z\n", argv[0]);
    } else {

        int x = atoi(argv[1]);
        int y = atoi(argv[2]);
        int z = atoi(argv[3]);

        printf("Tarai output: %d\n", tarai(x, y, z));
    }
}
