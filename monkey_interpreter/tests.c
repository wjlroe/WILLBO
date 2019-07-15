#include "ast/ast.h"
#include "lexer/lexer_test.c"
#include "minunit.h"
#include "parser/parser_test.c"

static void run_the_tests(TestRun* test_run) {
    // mu_run_test(test_next_token);
    mu_run_test(test_parser_let_statements);
}

RUN_TESTS(run_the_tests);
