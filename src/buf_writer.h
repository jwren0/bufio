#ifndef BUFIO_BUF_WRITER_H
#define BUFIO_BUF_WRITER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Default buffer size.
 */
#define BUFIO_BUFW_SIZE 8 * 1024

/**
 * A struct which holds data for a buffered writer.
 */
typedef struct {
    /**
     * The file being written to.
     */
    FILE *f;

    /**
     * The internal buffer.
     */
    char *buf;

    /**
     * The maximum number of elements in the internal buffer.
     */
    size_t buf_max;

    /**
     * The number of elements currently in the buffer.
     */
    size_t buf_stored;
} BufWriter;

/**
 * Initializes a BufWriter.
 *
 * @param br The buffered writer to initialize
 * @param f The file stream to write to.
 * @return zero on success, non-zero otherwise.
 */
int BufWriter_init(BufWriter *bw, FILE *f);

/**
 * Initializes a BufWriter with a specified buffer size.
 *
 * @param br The buffered writer to initialize.
 * @param f The file stream to write to.
 * @param size The quantity of chars the internal buffer can store.
 * @return zero on success, non-zero otherwise.
 */
int BufWriter_init_sized(BufWriter *bw, FILE *f, size_t size);

/**
 * Writes up to BufWriter.buf_max chars from the internal buffer
 * of a BufWriter into a file.
 *
 * @param br A pointer to a buffered writer.
 * @return zero on success, non-zero otherwise.
 */
int _BufWriter_write(BufWriter *bw);

/**
 * Reads a sequence of chars from the provided buffer
 * and writes them into a file.
 *
 * @param br A pointer to a buffered writer.
 * @param count The size of `buf`.
 * @param buf A pointer to the buffer to read from.
 * @return The quantity of chars read from buf and written to a file.
 */
size_t BufWriter_puts(BufWriter *bw, size_t count, char *buf);

/**
 * Writes a char to a file.
 *
 * @param br A pointer to a buffered writer.
 * @param c The char to write
 * @return zero on success, non-zero otherwise
 */
int BufWriter_putc(BufWriter *bw, char c);

/**
 * Frees all resources allocated to
 * a BufWriter, excluding the file stream.
 *
 * @param br A pointer to a buffered writer.
 */
void BufWriter_close(BufWriter *bw);

#endif // BUFIO_BUF_WRITER_H
