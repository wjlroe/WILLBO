#include "lexer.h"

void ReadChar(Lexer* Lexer) {
    if (Lexer->ReadPosition >= strlen(Lexer->Input)) {
        Lexer->CurrentChar = 0;
    } else {
        Lexer->CurrentChar = Lexer->Input[Lexer->ReadPosition];
    }
    Lexer->Position = Lexer->ReadPosition;
    Lexer->ReadPosition++;
}

void InitLexer(Lexer* Lexer, const char* Input) {
    size_t InputLen = strlen(Input);
    size_t InputSizeWithNull = InputLen + 1;
    Lexer->Input = malloc(InputSizeWithNull);
    if (Lexer->Input == NULL) {
        printf("Failed to allocate memory for Lexer input\n");
        exit(1);
    }
    strncpy(Lexer->Input, Input, InputLen);
    Lexer->Input[InputLen + 1] = '\0';

    ReadChar(Lexer);
}

Token NewToken(TokenType Type, char* Literal) {
    return (Token){.TokenType = Type, .Literal = Literal};
}

Token NextToken(Lexer* Lexer) {
    Token Token;

    switch (Lexer->CurrentChar) {
        case '=': {
            Token = NewToken(TOKEN_ASSIGN, "=");
        } break;
        case ';': {
            Token = NewToken(TOKEN_SEMICOLON, ";");
        } break;
        case '(': {
            Token = NewToken(TOKEN_LPAREN, "(");
        } break;
        case ')': {
            Token = NewToken(TOKEN_RPAREN, ")");
        } break;
        case ',': {
            Token = NewToken(TOKEN_COMMA, ",");
        } break;
        case '+': {
            Token = NewToken(TOKEN_PLUS, "+");
        } break;
        case '{': {
            Token = NewToken(TOKEN_LBRACE, "{");
        } break;
        case '}': {
            Token = NewToken(TOKEN_RBRACE, "}");
        } break;
        case 0: {
            Token.Literal = "";
            Token.TokenType = TOKEN_EOF;
        }
    }

    ReadChar(Lexer);

    return Token;
}
