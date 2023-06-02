#ifndef BUFIO_BUF_READER_H
#define BUFIO_BUF_READER_H

/**
 * A file containing typdefs and documentation
 * for BufReader.
 *
 * @author  jwren0
 * @version 2023-06-02
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Default buffer size.
 */
#define BUFIO_BUFR_SIZE 8 * 1024

/**
 * A struct which holds data for a buffered reader.
 */
typedef struct {
    /**
     * The file being read from.
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

    /**
     * The current position inside the buffer.
     */
    size_t cursor;
} BufReader;

/**
 * Initializes a BufReader.
 *
 * @param br The buffered reader to initialize
 * @param f The file stream to read from.
 * @return zero on success, non-zero otherwise.
 */
int BufReader_init(BufReader *br, FILE *f);

/**
 * Initializes a BufReader with a specified buffer size.
 *
 * @param br The buffered reader to initialize.
 * @param f The file stream to read from.
 * @param size The quantity of chars the internal buffer can store.
 * @return zero on success, non-zero otherwise.
 */
int BufReader_init_sized(BufReader *br, FILE *f, size_t size);

/**
 * Reads up to BufReader.buf_max chars into the internal buffer
 * of a BufReader.
 *
 * @param br A pointer to a buffered reader.
 * @return zero on success, non-zero otherwise.
 */
int _BufReader_read(BufReader *br);

/**
 * Reads a sequence of chars from a file
 * into the provided buffer.
 *
 * @param br A pointer to a buffered reader.
 * @param count The quantity of chars to read (including NULL byte).
 * @param buf A pointer to the buffer to write to.
 * @return The quantity of chars written to buf.
 */
size_t BufReader_gets(BufReader *br, size_t count, char *buf);

/**
 * Reads a char from a file.
 *
 * @param br A pointer to a buffered reader.
 * @param c A pointer to the char to write to
 * @return zero on success, non-zero otherwise
 */
int BufReader_getc(BufReader *br, char *c);

/**
 * Frees all resources allocated to
 * a BufReader, excluding the file stream.
 *
 * @param br A pointer to a buffered reader.
 */
void BufReader_close(BufReader *br);

#endif // BUFIO_BUF_READER_H
