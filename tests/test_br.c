/**
 * A file containing some basic tests for BufReader.
 *
 * @author  jwren0
 * @version 2023-06-12
 */

#include "../src/buf_reader.h"
#include "test.h"

/**
 * Ensures BufReader_init_sized and BufReader_close are working.
 */
void test_init_sized_close(void) {
    int actual;
    const size_t BUF_SIZE = 1024;
    BufReader br;
    FILE *f = fopen(TESTR_FILE, "r");
    focheck(f);

    actual = BufReader_init_sized(&br, f, BUF_SIZE);
    assert_eq(
        "[test_init_sized_close] BufReader_init_sized",
        0, actual
    );

    assert_peq(
        "[test_init_sized_close] BufReader.f",
        f, br.f
    );

    assert_not_null(
        "[test_init_sized_close] BufReader.buf",
        br.buf
    );

    assert_eq(
        "[test_init_sized_close] BufReader.buf_max",
        BUF_SIZE, br.buf_max
    );

    assert_eq(
        "[test_init_sized_close] BufReader.buf_stored",
        0, br.buf_stored
    )

    assert_eq(
        "[test_init_sized_close] BufReader.cursor",
        0, br.cursor
    );

    BufReader_close(&br);
    assert_null(
        "[test_init_sized_close] BufReader_close",
        br.buf
    );

    if (fclose(f) != 0) {
        fccheck(f);
    }
}

/**
 * Ensures BufReader_init is working.
 */
void test_init(void) {
    int actual;
    BufReader br;
    FILE *f = fopen(TESTR_FILE, "r");
    focheck(f);

    actual = BufReader_init(&br, f);
    assert_eq(
        "[test_init] BufReader_init",
        0, actual
    );

    assert_peq(
        "[test_init] BufReader.f",
        f, br.f
    );

    assert_not_null(
        "[test_init] BufReader.buf",
        br.buf
    );

    assert_eq(
        "[test_init] BufReader.buf_max",
        BUFIO_BUFR_SIZE, br.buf_max
    );

    assert_eq(
        "[test_init] BufReader.buf_stored",
        0, br.buf_stored
    )

    assert_eq(
        "[test_init] BufReader.cursor",
        0, br.cursor
    );

    BufReader_close(&br);
    assert_null(
        "[test_init] BufReader_close",
        br.buf
    );

    if (fclose(f) != 0) {
        fccheck(f);
    }
}

/**
 * Ensures _BufReader_read is working.
 */
void test_read(void) {
    int actual;
    size_t expected_count[] = {
        16, 16, 10,
    };
    char *expected_blocks[] = {
        "Hello, world!\nHe",
        "llo, BufReader!\n",
        "More data\n",
    };
    size_t expected_len = ARRAY_LEN(expected_count);
    size_t BUF_SIZE = 17;
    BufReader br;
    FILE *f = fopen(TESTR_FILE, "r");
    focheck(f);

    actual = BufReader_init_sized(&br, f, BUF_SIZE);
    assert_eq(
        "[test_read] BufReader_init_sized",
        0, actual
    );

    for (size_t i = 0; i < expected_len; i++) {
        actual = _BufReader_read(&br);
        assert_eq(
            "[test_read] _BufReader_read",
            0, actual
        );

        assert_eq(
            "[test_read] _BufReader_read count",
            expected_count[i], strlen(br.buf)
        );

        assert_streq(
            "[test_read] _BufReader_read block",
            expected_blocks[i], br.buf
        );
    }

    BufReader_close(&br);

    if (fclose(f) != 0) {
        fccheck(f);
    }
}

/**
 * Ensures BufReader_getc is working.
 */
void test_getc(void) {
    int actual;
    char expected[] = "Hello, world!\nHello, BufReader!\nMore data\n";
    char c;
    size_t expected_len = strlen(expected);
    BufReader br;
    FILE *f = fopen(TESTR_FILE, "r");
    focheck(f);

    actual = BufReader_init(&br, f);
    assert_eq(
        "[test_getc] BufReader_init",
        0, actual
    );

    puts("[test_getc] BufReader_getc tests quietly, inconsistencies will be shown");

    for (size_t i = 0; i < expected_len; i++) {
        actual = BufReader_getc(&br, &c);

        if (actual != 0) {
            assert_eq(
                "[test_getc] BufReader_getc",
                0, actual
            );
        }

        if (c == expected[i]) {
            continue;
        }

        assert_eq(
            "[test_getc] BufReader_getc value read",
            expected[i], c
        );
    }

    BufReader_close(&br);

    if (fclose(f) != 0) {
        fccheck(f);
    }
}

/**
 * Ensures BufReader_gets is working.
 */
void test_gets(void) {
    int actual;
    size_t actual_nread;
    char expected[] = "Hello, world!\nHello, BufReader!\nMore data\n";
    size_t expected_len = strlen(expected);
    char buf[expected_len + 1];
    BufReader br;
    FILE *f = fopen(TESTR_FILE, "r");
    focheck(f);

    actual = BufReader_init(&br, f);
    assert_eq(
        "[test_gets] BufReader_init",
        0, actual
    );

    actual_nread = BufReader_gets(&br, expected_len + 1, buf);
    assert_eq(
        "[test_gets] BufReader_gets",
        expected_len, actual_nread
    );

    assert_streq(
        "[test_gets] BufReader_gets streq",
        expected, buf
    );

    BufReader_close(&br);

    if (fclose(f) != 0) {
        fccheck(f);
    }
}

int main(void) {
    test_init_sized_close();
    test_init();
    test_read();
    test_getc();
    test_gets();
}
