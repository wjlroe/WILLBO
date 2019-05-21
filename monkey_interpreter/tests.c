#include "lexer/lexer_test.c"
#include "minunit.h"

static void run_the_tests(TestRun *test_run) {
    mu_run_test(test_next_token);
}

RUN_TESTS(run_the_tests);
