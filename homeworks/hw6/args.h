#ifndef _ARGS_H_
#define _ARGS_H_

/** 
 * Returns a malloc-allocated c string containing the
 * contents of the file to be assembled.
 */
char* get_content(int argc, char* argv[]);

/**
 * Fetches the contents of the file in the provided path
 * and converts it into a c-string allocated with malloc.
 * Returns NULL if the file is not found
 */
char* file_to_string(char* path);

#endif
