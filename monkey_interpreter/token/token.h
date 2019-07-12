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
    TOKEN_EQ,
    TOKEN_NOT_EQ,

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
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_RETURN,
} TokenType;

typedef struct {
    TokenType TokenType;
    char* Literal;
} Token;

typedef struct {
    char* Literal;
    TokenType TokenType;
} KeywordMapping;

static const KeywordMapping KEYWORDS[] = {
    {"fn", TOKEN_FUNCTION},
    {"let", TOKEN_LET},
    {"true", TOKEN_TRUE},
    {"false", TOKEN_FALSE},
    {"if", TOKEN_IF},
    {"else", TOKEN_ELSE},
    {"return", TOKEN_RETURN},
};

const KeywordMapping* LookupKeyword(char* Ident);
void FreeToken(Token Token);

#endif
