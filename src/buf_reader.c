/**
 * A file containing the source code for BufReader functions.
 *
 * @author  jwren0
 * @version 2023-06-02
 */

#include "buf_reader.h"

int BufReader_init(BufReader *br, FILE *f) {
    return BufReader_init_sized(br, f, BUFIO_BUFR_SIZE);
}

int BufReader_init_sized(BufReader *br, FILE *f, size_t size) {
    char *buf;

    if (br == NULL || f == NULL || size < 1) {
        return 1;
    }

    buf = calloc(size, sizeof(char));

    if (buf == NULL) {
        perror("calloc");
        return 1;
    }

    br->f = f;
    br->buf = buf;
    br->buf_max = size;
    br->buf_stored = 0;
    br->cursor = 0;

    return 0;
}

int _BufReader_read(BufReader *br) {
    if (br == NULL || br->f == NULL || br->buf == NULL) {
        return 1;
    }

    if (feof(br->f) != 0) {
        return 1;
    }

    // Reserve a byte for NULL
    size_t nread = fread(br->buf, sizeof(char), br->buf_max - 1, br->f);

    if (nread < br->buf_max - 1 && ferror(br->f) != 0) {
        perror("fread");
        return 1;
    }

    br->buf[nread] = '\0';
    br->buf_stored = nread;
    br->cursor = 0;

    return 0;
}

size_t BufReader_gets(BufReader *br, size_t count, char *buf) {
    size_t count_left, count_req, nread = 0;

    if (br == NULL || br->f == NULL || buf == NULL || count < 1) {
        return 0;
    }

    // If there's nothing more to get, return
    if (br->cursor >= br->buf_stored && feof(br->f) != 0) {
        return 0;
    }

    // Reserve a byte for NULL
    while (nread < count - 1) {
        if (br->cursor >= br->buf_stored && _BufReader_read(br) != 0) {
            break;
        }

        // Number of chars left in the buffer and requested to read
        count_left = br->buf_stored - br->cursor;
        count_req = count - nread - 1;

        // If too many chars are requested, only copy what's left
        if (count_left < count_req) {
            count_req = count_left;
        }

        memcpy(buf + nread, br->buf + br->cursor, count_req);

        // Update cursor position and number of chars read
        br->cursor += count_req;
        nread += count_req;
    }

    // Ensure NULL terminated
    buf[nread] = '\0';

    return nread;
}

int BufReader_getc(BufReader *br, char *c) {
    if (br == NULL || br->buf == NULL) {
        return 1;
    }

    if (br->cursor >= br->buf_stored && _BufReader_read(br) != 0) {
        return 1;
    }

    *c = br->buf[br->cursor++];
    return 0;
}

void BufReader_close(BufReader *br) {
    if (br == NULL || br->buf == NULL) {
        return;
    }

    free(br->buf);
    br->buf = NULL;
}
