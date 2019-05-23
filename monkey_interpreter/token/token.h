#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_ILLEGAL,
    TOKEN_EOF,

    TOKEN_IDENT,  // add, foobar, x, y, ...
    TOKEN_INT,    // 123456

    // Operators
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_BANG,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_LT,
    TOKEN_GT,

    // Delimiters
    TOKEN_COMMA,
    TOKEN_SEMICOLON,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    // Keywords
    TOKEN_FUNCTION,
    TOKEN_LET,
} TokenType;

typedef struct {
    TokenType TokenType;
    char* Literal;
} Token;

typedef struct {
    char* Literal;
    TokenType TokenType;
} KeywordMapping;

const KeywordMapping KEYWORDS[] = {
    {"fn", TOKEN_FUNCTION},
    {"let", TOKEN_LET},
};

TokenType LookupIdent(char* Ident);

#endif
