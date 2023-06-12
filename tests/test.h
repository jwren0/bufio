#ifndef BUFIO_TESTS_TEST_H
#define BUFIO_TESTS_TEST_H

/**
 * A file containing some test macros.
 *
 * @author  jwren0
 * @version 2023-06-12
 */

#define TESTR_FILE "tests/test_br.txt"
#define TESTW_FILE "tests/test_bw.txt"

#define ARRAY_LEN(a) sizeof(a)/sizeof(*a);

#define focheck(f) {     \
    if (f == NULL) {     \
        perror("fopen"); \
        exit(1);         \
    }                    \
}

#define fccheck(f) {      \
    if (ferror(f) != 0) { \
        perror("fclose"); \
        exit(1);          \
    }                     \
}

#define assert_eq(s, a, b) {                      \
    if (a != b) {                                 \
        fprintf(                                  \
            stderr,                               \
            "%s: \033[31mFailed\033[0m. "         \
            "Expected '%#x', got '%#x'\n",        \
            s, (int) a, (int) b                   \
        );                                        \
    } else {                                      \
        printf("%s: \033[32mPassed\033[0m\n", s); \
    }                                             \
}

#define assert_ne(s, a, b) {                      \
    if (a == b) {                                 \
        fprintf(                                  \
            stderr,                               \
            "%s: \033[31mFailed\033[0m. "         \
            "Didn't expect '%#x', got '%#x'\n",   \
            s, (int) a, (int) b                   \
        );                                        \
    } else {                                      \
        printf("%s: \033[32mPassed\033[0m\n", s); \
    }                                             \
}

#define assert_peq(s, a, b) {                     \
    if (a != b) {                                 \
        fprintf(                                  \
            stderr,                               \
            "%s: \033[31mFailed\033[0m. "         \
            "Expected '%p', got '%p'\n",          \
            s, (void *) a, (void *) b             \
        );                                        \
    } else {                                      \
        printf("%s: \033[32mPassed\033[0m\n", s); \
    }                                             \
}

#define assert_pne(s, a, b) {                     \
    if (a == b) {                                 \
        fprintf(                                  \
            stderr,                               \
            "%s: \033[31mFailed\033[0m. "         \
            "Didn't expect '%p', got '%p'\n",     \
            s, (void *) a, (void *) b             \
        );                                        \
    } else {                                      \
        printf("%s: \033[32mPassed\033[0m\n", s); \
    }                                             \
}

#define assert_streq(s, a, b) {                   \
    if (strcmp(a, b) != 0) {                      \
        fprintf(                                  \
            stderr,                               \
            "%s: \033[31mFailed\033[0m. "         \
            "Expected '%s', got '%s'\n",          \
            s, a, b                               \
        );                                        \
    } else {                                      \
        printf("%s: \033[32mPassed\033[0m\n", s); \
    }                                             \
}

#define assert_strne(s, a, b) {                   \
    if (strcmp(a, b) == 0) {                      \
        fprintf(                                  \
            stderr,                               \
            "%s: \033[31mFailed\033[0m. "         \
            "Didn't expect '%s', got '%s'\n",     \
            s, a, b                               \
        );                                        \
    } else {                                      \
        printf("%s: \033[32mPassed\033[0m\n", s); \
    }                                             \
}

#define assert_null(s, a) {                       \
    if (a != NULL) {                              \
        fprintf(                                  \
            stderr,                               \
            "%s: \033[31mFailed\033[0m. "         \
            "Expected NULL, got %p\n",            \
            s, (void *) a                         \
        );                                        \
    } else {                                      \
        printf("%s: \033[32mPassed\033[0m\n", s); \
    }                                             \
}

#define assert_not_null(s, a) {                   \
    if (a == NULL) {                              \
        fprintf(                                  \
            stderr,                               \
            "%s: \033[31mFailed\033[0m. "         \
            "Didn't expect NULL, got %p\n",       \
            s, (void *) a                         \
        );                                        \
    } else {                                      \
        printf("%s: \033[32mPassed\033[0m\n", s); \
    }                                             \
}

#endif // BUFIO_TESTS_TEST_H
