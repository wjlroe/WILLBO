#ifndef _minunit_h
#define _minunit_h

#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"

#define test static void

#define mu_assert(test, message...) \
    test_run->asserts_run++;        \
    if (!(test)) {                  \
        test_run->asserts_failed++; \
        log_err(message);           \
        return;                     \
    }

#define mu_assert_equal(actual, expected, message...) \
    mu_assert(expected == actual, message);

#define mu_assert_changed(value, how_much, test, message) \
    {                                                     \
        int initial = (value);                            \
        (test);                                           \
        int newval = (value);                             \
        int diff = newval - initial;                      \
        mu_assert(diff == how_much, message);             \
    }

#define mu_assert_not_changed(value, test, message) \
    mu_assert_changed(value, 0, test, message);

#define mu_assert_str_equal(actual, expected) \
    mu_assert(strcmp(actual, expected) == 0,  \
              "'%s' does not equal '%s'",     \
              actual,                         \
              expected);

#define mu_assert_array_equal(                                   \
    comparison_fn, actual, actual_len, expected, expected_len)   \
    {                                                            \
        mu_assert_int_equal((int)actual_len, (int)expected_len); \
        for (size_t i = 0; i < actual_len; i++) {                \
            mu_assert(comparison_fn(&actual[i], &expected[i]),   \
                      "Element at index %lu not equal",          \
                      i);                                        \
        }                                                        \
    }

#define mu_assert_int_equal(actual, expected) \
    mu_assert(expected == actual,             \
              "%s (%i) does not equal %i",    \
              #actual,                        \
              actual,                         \
              expected);

#define mu_run_test(test)             \
    printf("\n-----%s\n", " " #test); \
    test(test_run);                   \
    test_run->tests_run++;

#define test_results(test_run)                         \
    printf("%d tests run, %d assertions, %d failed\n", \
           test_run->tests_run,                        \
           test_run->asserts_run,                      \
           test_run->asserts_failed);

#define check_test_args(num_args, arguments)

typedef struct {
    int tests_run;
    int asserts_run;
    int asserts_passed;
    int asserts_failed;
} TestRun;

#define RUN_TESTS(name)                                \
    int main(int argc, char* argv[]) {                 \
        argc = 1;                                      \
        if (argc > 1) {                                \
            char* test_to_run = argv[1];               \
            printf("Run the %s test", test_to_run);    \
        }                                              \
        TestRun* test_run = malloc(sizeof(TestRun));   \
        test_run->tests_run = 0;                       \
        test_run->asserts_run = 0;                     \
        test_run->asserts_passed = 0;                  \
        test_run->asserts_failed = 0;                  \
        printf("----- RUNNING: %s\n", argv[0]);        \
        name(test_run);                                \
        if (test_run->asserts_failed != 0) {           \
            printf(ANSI_COLOR_RED);                    \
            printf("FAILED:\n");                       \
            test_results(test_run);                    \
            printf(ANSI_COLOR_RESET "\n");             \
        } else {                                       \
            printf(ANSI_COLOR_GREEN);                  \
            printf("ALL TESTS PASSED\n");              \
            test_results(test_run);                    \
            printf(ANSI_COLOR_RESET "\n");             \
        }                                              \
        int exit_code = test_run->asserts_failed != 0; \
        free(test_run);                                \
        exit(exit_code);                               \
    }

#endif
