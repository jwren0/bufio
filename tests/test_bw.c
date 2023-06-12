/**
 * A file containing some basic tests for BufWriter.
 *
 * @author jwren0
 * @version 2023-06-12
 */

#include "../src/buf_writer.h"
#include "test.h"

/**
 * Ensures that the content within a file is as expected.
 */
void test_check_content(FILE *f, char *expected, size_t expected_len) {
    size_t nread;
    char actual[expected_len + 1];

    memset(actual, 0, expected_len + 1);

    nread = fread(actual, sizeof(char), expected_len + 1, f);

    if (nread < expected_len && ferror(f) != 0) {
        perror("fread");
    }

    assert_eq(
        "  [test_check_content] Read written content length",
        expected_len, nread
    )

    assert_streq(
        "  [test_check_content] Read written content",
        expected, actual
    );
}

/**
 * Ensures BufWriter_init_sized and BufWriter_close are working.
 */
void test_init_sized_close(void) {
    int actual;
    const size_t BUF_SIZE = 1024;
    BufWriter bw;
    FILE *f = fopen(TESTW_FILE, "w");
    focheck(f);

    actual = BufWriter_init_sized(&bw, f, BUF_SIZE);
    assert_eq(
        "[test_init_sized_close] BufWriter_init_sized",
        0, actual
    );

    assert_peq(
        "[test_init_sized_close] BufWriter.f",
        f, bw.f
    );

    assert_not_null(
        "[test_init_sized_close] BufWriter.buf",
        bw.buf
    );

    assert_eq(
        "[test_init_sized_close] BufWriter.buf_max",
        BUF_SIZE, bw.buf_max
    );

    assert_eq(
        "[test_init_sized_close] BufWriter.buf_stored",
        0, bw.buf_stored
    )

    BufWriter_close(&bw);
    assert_null(
        "[test_init_sized_close] BufWriter_close",
        bw.buf
    );

    if (fclose(f) != 0) {
        fccheck(f);
    }
}

/**
 * Ensures BufWriter_init is working.
 */
void test_init(void) {
    int actual;
    BufWriter bw;
    FILE *f = fopen(TESTW_FILE, "w");
    focheck(f);

    actual = BufWriter_init(&bw, f);
    assert_eq(
        "[test_init] BufWriter_init_sized",
        0, actual
    );

    assert_peq(
        "[test_init] BufWriter.f",
        f, bw.f
    );

    assert_not_null(
        "[test_init] BufWriter.buf",
        bw.buf
    );

    assert_eq(
        "[test_init] BufWriter.buf_max",
        BUFIO_BUFW_SIZE, bw.buf_max
    );

    assert_eq(
        "[test_init] BufWriter.buf_stored",
        0, bw.buf_stored
    )

    BufWriter_close(&bw);
    assert_null(
        "[test_init] BufWriter_close",
        bw.buf
    );

    if (fclose(f) != 0) {
        fccheck(f);
    }
}

/**
 * Ensures _BufWriter_write is working.
 */
void test_write(void) {
    int actual;
    char expected[] = "Hello world!\nHello BufWriter!\nMore data\n";
    size_t expected_len = strlen(expected);
    BufWriter bw;
    FILE *f = fopen(TESTW_FILE, "w");
    focheck(f);

    actual = BufWriter_init_sized(&bw, f, expected_len);
    assert_eq(
        "[test_write] BufWriter_init_sized",
        0, actual
    );

    memcpy(bw.buf, expected, expected_len);
    bw.buf_stored = expected_len;

    assert_eq(
        "[test_write] BufWriter_write buf_stored before",
        expected_len, bw.buf_stored
    );

    _BufWriter_write(&bw);

    assert_eq(
        "[test_write] BufWriter_write buf_stored after",
        0, bw.buf_stored
    );

    BufWriter_close(&bw);

    if (fclose(f) != 0) {
        fccheck(f);
    }

    // Check contents of file
    f = fopen(TESTW_FILE, "r");
    focheck(f);

    test_check_content(f, expected, expected_len);

    if (fclose(f) != 0) {
        fccheck(f);
    }
}

/**
 * Ensures BufWriter_putc is working.
 */
void test_putc(void) {
    int actual;
    char expected[] = "Hello world!\nHello BufWriter!\nMore data\n";
    size_t expected_len = strlen(expected);
    BufWriter bw;
    FILE *f = fopen(TESTW_FILE, "w");
    focheck(f);

    actual = BufWriter_init(&bw, f);
    assert_eq(
        "[test_putc] BufWriter_init",
        0, actual
    );

    puts("[test_putc] BufWriter_putc tests quietly, inconsistencies will be shown");

    for (size_t i = 0; i < expected_len; i++) {
        actual = BufWriter_putc(&bw, expected[i]);

        if (actual != 0) {
            assert_eq(
                "[test_putc] BufWriter_putc",
                0, actual
            );
        }

        if (bw.buf[bw.buf_stored - 1] == expected[i]) {
            continue;
        }

        assert_eq(
            "[test_putc] BufWriter_putc value written",
            expected[i], bw.buf[bw.buf_stored - 1]
        );
    }

    BufWriter_close(&bw);

    if (fclose(f) != 0) {
        fccheck(f);
    }

    // Check contents of file
    f = fopen(TESTW_FILE, "r");
    focheck(f);

    test_check_content(f, expected, expected_len);

    if (fclose(f) != 0) {
        fccheck(f);
    }
}

/**
 * Ensures BufWriter_puts is working.
 */
void test_puts(void) {
    int actual;
    char expected[] = "Hello world!\nHello BufWriter!\nMore data\n";
    size_t expected_len = strlen(expected);
    BufWriter bw;
    FILE *f = fopen(TESTW_FILE, "w");
    focheck(f);

    actual = BufWriter_init(&bw, f);
    assert_eq(
        "[test_puts] BufWriter_init",
        0, actual
    );

    actual = BufWriter_puts(&bw, expected_len, expected);
    assert_eq(
        "[test_puts] BufWriter_puts",
        0, actual
    );

    BufWriter_close(&bw);

    if (fclose(f) != 0) {
        fccheck(f);
    }

    // Check contents of file
    f = fopen(TESTW_FILE, "r");
    focheck(f);

    test_check_content(f, expected, expected_len);

    if (fclose(f) != 0) {
        fccheck(f);
    }
}

int main(void) {
    test_init_sized_close();
    test_init();
    test_write();
    test_putc();
    test_puts();
}
