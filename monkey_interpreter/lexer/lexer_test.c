#include "lexer.c"
#include "test_inc.h"
#include "token/token.c"

test test_next_token(TestRun* test_run) {
    char* input =
        "let five = 5;\n"
        "let ten = 10;\n"
        "\n"
        "let add = fn(x, y) {\n"
        " x + y;\n"
        "};\n"
        "\n"
        "let result = add(five, ten);\n"
        "!-/*5;\n"
        "5 < 10 > 5;\n"
        "\n"
        "if (5 < 10) {\n"
        "  return true;\n"
        "} else {\n"
        "  return false;\n"
        "}\n";
    Token expected[] = {
        {.TokenType = TOKEN_LET, "let"},
        {.TokenType = TOKEN_IDENT, "five"},
        {.TokenType = TOKEN_ASSIGN, "="},
        {.TokenType = TOKEN_INT, "5"},
        {.TokenType = TOKEN_SEMICOLON, ";"},
        {.TokenType = TOKEN_LET, "let"},
        {.TokenType = TOKEN_IDENT, "ten"},
        {.TokenType = TOKEN_ASSIGN, "="},
        {.TokenType = TOKEN_INT, "10"},
        {.TokenType = TOKEN_SEMICOLON, ";"},
        {.TokenType = TOKEN_LET, "let"},
        {.TokenType = TOKEN_IDENT, "add"},
        {.TokenType = TOKEN_ASSIGN, "="},
        {.TokenType = TOKEN_FUNCTION, "fn"},
        {.TokenType = TOKEN_LPAREN, "("},
        {.TokenType = TOKEN_IDENT, "x"},
        {.TokenType = TOKEN_COMMA, ","},
        {.TokenType = TOKEN_IDENT, "y"},
        {.TokenType = TOKEN_RPAREN, ")"},
        {.TokenType = TOKEN_LBRACE, "{"},
        {.TokenType = TOKEN_IDENT, "x"},
        {.TokenType = TOKEN_PLUS, "+"},
        {.TokenType = TOKEN_IDENT, "y"},
        {.TokenType = TOKEN_SEMICOLON, ";"},
        {.TokenType = TOKEN_RBRACE, "}"},
        {.TokenType = TOKEN_SEMICOLON, ";"},
        {.TokenType = TOKEN_LET, "let"},
        {.TokenType = TOKEN_IDENT, "result"},
        {.TokenType = TOKEN_ASSIGN, "="},
        {.TokenType = TOKEN_IDENT, "add"},
        {.TokenType = TOKEN_LPAREN, "("},
        {.TokenType = TOKEN_IDENT, "five"},
        {.TokenType = TOKEN_COMMA, ","},
        {.TokenType = TOKEN_IDENT, "ten"},
        {.TokenType = TOKEN_RPAREN, ")"},
        {.TokenType = TOKEN_SEMICOLON, ";"},
        {.TokenType = TOKEN_BANG, "!"},
        {.TokenType = TOKEN_MINUS, "-"},
        {.TokenType = TOKEN_SLASH, "/"},
        {.TokenType = TOKEN_ASTERISK, "*"},
        {.TokenType = TOKEN_INT, "5"},
        {.TokenType = TOKEN_SEMICOLON, ";"},
        {.TokenType = TOKEN_INT, "5"},
        {.TokenType = TOKEN_LT, "<"},
        {.TokenType = TOKEN_INT, "10"},
        {.TokenType = TOKEN_GT, ">"},
        {.TokenType = TOKEN_INT, "5"},
        {.TokenType = TOKEN_SEMICOLON, ";"},
        {.TokenType = TOKEN_IF, "if"},
        {.TokenType = TOKEN_LPAREN, "("},
        {.TokenType = TOKEN_INT, "5"},
        {.TokenType = TOKEN_LT, "<"},
        {.TokenType = TOKEN_INT, "10"},
        {.TokenType = TOKEN_RPAREN, ")"},
        {.TokenType = TOKEN_LBRACE, "{"},
        {.TokenType = TOKEN_RETURN, "return"},
        {.TokenType = TOKEN_TRUE, "true"},
        {.TokenType = TOKEN_SEMICOLON, ";"},
        {.TokenType = TOKEN_RBRACE, "}"},
        {.TokenType = TOKEN_ELSE, "else"},
        {.TokenType = TOKEN_LBRACE, "{"},
        {.TokenType = TOKEN_RETURN, "return"},
        {.TokenType = TOKEN_FALSE, "false"},
        {.TokenType = TOKEN_SEMICOLON, ";"},
        {.TokenType = TOKEN_RBRACE, "}"},
        {.TokenType = TOKEN_EOF, ""},
    };
    size_t expected_len = sizeof(expected) / sizeof(expected[0]);
    Lexer* lexer = malloc(sizeof(Lexer));
    InitLexer(lexer, input);
    mu_assert_str_equal(lexer->Input, input);
    for (int i = 0; i < expected_len; i++) {
        Token token = NextToken(lexer);

        mu_assert_equal(token.TokenType,
                        expected[i].TokenType,
                        "[i=%d] actual TokenType: %d (literal: '%s') does not "
                        "equal expected: %d (literal: '%s')",
                        i,
                        token.TokenType,
                        token.Literal,
                        expected[i].TokenType,
                        expected[i].Literal);
        mu_assert_str_equal(token.Literal, expected[i].Literal);
    }
}
