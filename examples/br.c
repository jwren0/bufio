/**
 * An example of using BufReader.
 *
 * @author  jwren0
 * @version 2023-06-12
 *
 * Make sure the library has been built before compiling and running.
 *
 * Compile:
 * ```
 * examples/ $ gcc -L.. -lbufio br.c -o br
 * ```
 *
 * Run:
 * ```
 * examples/ $ LD_LIBRARY_PATH=.. ./br
 * ```
 */

#include <stdio.h>
#include "../src/buf_reader.h"

int main(void) {
    char c;
    BufReader br;
    FILE *f = fopen("file.txt", "r");

    // Check fopen worked
    if (f == NULL) {
        perror("fopen");
        return 1;
    }

    // Try initializing BufReader
    if (BufReader_init(&br, f) != 0) {
        fprintf(stderr, "Failed initializing BufReader");
        goto cleanup;
    }

    // Keep reading chars until unable to
    while (BufReader_getc(&br, &c) == 0) {
        // Do something with c
    }

    // Check if EOF was reached
    if (feof(f) == 0) {
        fprintf(stderr, "Failed to reach EOF");
        goto cleanup;
    }

cleanup:
    BufReader_close(&br);

    // Check fclose worked
    if (fclose(f) != 0) {
        perror("fclose");
        return 1;
    }
}
