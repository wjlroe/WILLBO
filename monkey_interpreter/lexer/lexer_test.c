#include "lexer.c"
#include "test_inc.h"
#include "token/token.c"

test test_next_token(TestRun* test_run) {
    char* input = "=+(){},;";
    Token expected[] = {
        {.TokenType = TOKEN_ASSIGN, "="},
        {.TokenType = TOKEN_PLUS, "+"},
        {.TokenType = TOKEN_LPAREN, "("},
        {.TokenType = TOKEN_RPAREN, ")"},
        {.TokenType = TOKEN_LBRACE, "{"},
        {.TokenType = TOKEN_RBRACE, "}"},
        {.TokenType = TOKEN_COMMA, ","},
        {.TokenType = TOKEN_SEMICOLON, ";"},
        {.TokenType = TOKEN_EOF, ""},
    };
    size_t expected_len = sizeof(expected) / sizeof(expected[0]);
    Lexer* lexer = malloc(sizeof(Lexer));
    InitLexer(lexer, input);
    mu_assert_str_equal(lexer->Input, input);
    for (int i = 0; i < expected_len; i++) {
        Token token = NextToken(lexer);

        mu_assert_equal(
            token.TokenType,
            expected[i].TokenType,
            "[i=%d, ch='%c'] actual TokenType: %d does not equal expected: %d",
            i,
            input[i],
            token.TokenType,
            expected[i].TokenType);
        mu_assert_str_equal(token.Literal, expected[i].Literal);
    }
}
