// mem-chase.h

// This is the only file that you may modify for Laboratory 1.

// There are two function defined in this file.  You, as a cs350c
// student, must write C code to initialize the array "mem" so the
// program in "mem-chase.c" runs both as fast as possible and as
// slow as possible.

#define PAGE_SIZE          (4096)
#define L1_SIZE_PAGES      (1)
#define L2_SIZE_PAGES      (16)
#define L3_SIZE_PAGES      (128)

#define SLOW_MODE 1

void init_data_fast( unsigned long int mem[], unsigned long int size ) {
    // Simple, in-order accesses
    unsigned long int i;

    for (i = 0; i < size; i++)
        mem[i] = (i + 1);

    // And now, we overwrite the final entry
    mem[ size - 1 ] = 0;
}

unsigned long int div_up(unsigned long int a, unsigned long int b) {
    return 1 + ((a - 1) / b);
}

void init_data_slow( unsigned long int mem[], unsigned long int size ) {
    char first = 1;
    unsigned long int prevIdx = 0;
    unsigned long int elems_per_page = PAGE_SIZE / sizeof(unsigned long int);
    unsigned long int pages_segments = div_up(size*sizeof(unsigned long int), PAGE_SIZE);
    unsigned long int l1_pages = L1_SIZE_PAGES;
    unsigned long int l1_segments = L2_SIZE_PAGES / L1_SIZE_PAGES;
    unsigned long int l2_segments = L3_SIZE_PAGES / L2_SIZE_PAGES;
    unsigned long int l3_segments = div_up(pages_segments, L3_SIZE_PAGES);

    for (unsigned long int elem = 0; elem < elems_per_page; elem++) {
        for (unsigned long int page = 0; page < l1_pages; page++) {
            for (unsigned long int l1_seg = 0; l1_seg < l1_segments; l1_seg++) {
                for (unsigned long int l2_seg = 0; l2_seg < l2_segments; l2_seg++) {
                    for (unsigned long int l3_seg = 0; l3_seg < l3_segments; l3_seg++) {

                        unsigned long int page_num = page + l1_seg*L1_SIZE_PAGES + l2_seg*L2_SIZE_PAGES + l3_seg*L3_SIZE_PAGES;
                        unsigned long int cIdx = elem + page_num*PAGE_SIZE/sizeof(unsigned long int);

                        if (cIdx < size) {
                            if (!first) {
                                mem[prevIdx] = cIdx;
                            }
                            prevIdx = cIdx;
                            first = 0;
                        }
                    }
                }
            }
        }
    }

    mem[prevIdx] = 0;
}
