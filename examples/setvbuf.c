/**
 * An example implementation of modifying the buffer used for I/O
 * operations with setvbuf to improve performance.
 *
 * @author  jwren0
 * @version 2023-06-02
 */

#include <stdio.h>

int main(void) {
    int c;
    size_t BUF_SIZE = 8 * 1024;
    char buf[BUF_SIZE];
    FILE *f = fopen("file.txt", "r");

    if (f == NULL) {
        perror("fopen");
        return 1;
    }

    // Change the stream "f" to use the buffer "buf"
    // in _IOFBF mode (full buffering, refer to setvbuf(3)).
    // The size of the buffer is also provided.
    setvbuf(f, buf, _IOFBF, BUF_SIZE);

    // Iterate through all characters in the file.
    for (;;) {
        c = fgetc(f);

        // Instead of calling feof on every iteration,
        // only call if c is EOF.
        if (c == EOF && feof(f) != 0) {
            break;
        }
    }

    if (fclose(f) != 0) {
        perror("fclose");
        return 1;
    }
}
