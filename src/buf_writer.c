#include "buf_writer.h"

int BufWriter_init(BufWriter *bw, FILE *f) {
    return BufWriter_init_sized(bw, f, BUFIO_BUFW_SIZE);
}

int BufWriter_init_sized(BufWriter *bw, FILE *f, size_t size) {
    char *buf;

    if (bw == NULL || f == NULL || size < 1) {
        return 1;
    }

    buf = calloc(size, sizeof(char));

    if (buf == NULL) {
        perror("calloc");
        return 1;
    }

    bw->f = f;
    bw->buf = buf;
    bw->buf_max = size;
    bw->buf_stored = 0;

    return 0;
}

int _BufWriter_write(BufWriter *bw) {
    if (bw == NULL || bw->f == NULL || bw->buf == NULL) {
        return 1;
    }

    size_t nwrite = fwrite(bw->buf, sizeof(char), bw->buf_stored, bw->f);

    if (nwrite < bw->buf_stored && ferror(bw->f) != 0) {
        perror("fwrite");
        return 1;
    }

    bw->buf_stored = 0;

    return 0;
}

size_t BufWriter_puts(BufWriter *bw, size_t count, char *buf) {
    size_t count_copy, nwrite = 0;

    if (bw == NULL || bw->f == NULL || buf == NULL || count < 1) {
        return 0;
    }

    while (count > 0) {
        // Whenever internal buffer fills, write to file
        if (bw->buf_stored >= bw->buf_max && _BufWriter_write(bw) != 0) {
            return nwrite;
        }

        // Copy as many chars as possible to internal buffer
        count_copy = bw->buf_max - bw->buf_stored;

        // If this count is too high, only copy what's left
        if (count_copy > count) {
            count_copy = count;
        }

        memcpy(bw->buf + bw->buf_stored, buf, count_copy);

        // Keep track
        count -= count_copy;
        bw->buf_stored += count_copy;
    }

    return nwrite;
}

int BufWriter_putc(BufWriter *bw, char c) {
    if (bw == NULL || bw->buf == NULL) {
        return 1;
    }

    if (bw->buf_stored >= bw->buf_max && _BufWriter_write(bw) != 0) {
        return 1;
    }

    bw->buf[bw->buf_stored++] = c;
    return 0;
}

void BufWriter_close(BufWriter *bw) {
    if (bw == NULL || bw->buf == NULL) {
        return;
    }

    free(bw->buf);
    bw->buf = NULL;
}
