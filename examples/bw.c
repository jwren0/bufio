/**
 * An example of using BufWriter.
 *
 * @author  jwren0
 * @version 2023-06-12
 *
 * Make sure the library has been built before compiling and running.
 *
 * Compile:
 * ```
 * examples/ $ gcc -L.. -lbufio bw.c -o bw
 * ```
 *
 * Run:
 * ```
 * examples/ $ LD_LIBRARY_PATH=.. ./bw
 * ```
 */

#include <stdio.h>
#include "../src/buf_writer.h"

int main(void) {
    char content[] = "Hello from BufWriter!\n";
    size_t content_len = strlen(content);
    BufWriter bw;
    FILE *f = fopen("file_bw.txt", "w");

    // Check fopen worked
    if (f == NULL) {
        perror("fopen");
        return 1;
    }

    // Try initializing BufWriter
    if (BufWriter_init(&bw, f) != 0) {
        fprintf(stderr, "Failed initializing BufWriter\n");
        goto cleanup;
    }

    // Keep writing chars until unable to
    for (size_t i = 0; i < content_len; i++) {
        if (BufWriter_putc(&bw, content[i]) != 0) {
            fprintf(stderr, "Failed writing character '%c' to buffer", content[i]);
            goto cleanup;
        }
    }

cleanup:
    BufWriter_close(&bw);

    // Check fclose worked
    if (fclose(f) != 0) {
        perror("fclose");
        return 1;
    }
}
