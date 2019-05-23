#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>

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

bool IsLetter(char Character) {
    return (isalpha(Character) || Character == '_');
}

char* ReadIdentifier(Lexer* Lexer) {
    int Position = Lexer->Position;
    while (IsLetter(Lexer->CurrentChar)) {
        ReadChar(Lexer);
    }
    int SubStringSize = Lexer->Position - Position;
    char* Result = malloc(SubStringSize + 1);  // plus null terminator
    char* FromInput = Lexer->Input + Position;
    memcpy(Result, FromInput, SubStringSize);
    return Result;
}

char* ReadNumber(Lexer* Lexer) {
    int Position = Lexer->Position;
    while (isdigit(Lexer->CurrentChar)) {
        ReadChar(Lexer);
    }
    int SubStringSize = Lexer->Position - Position;
    char* Result = malloc(SubStringSize + 1);  // plus null terminator
    char* FromInput = Lexer->Input + Position;
    memcpy(Result, FromInput, SubStringSize);
    return Result;
}

void SkipWhitespace(Lexer* Lexer) {
    while ((Lexer->CurrentChar == ' ') || (Lexer->CurrentChar == '\t') ||
           (Lexer->CurrentChar == '\n') || (Lexer->CurrentChar == '\r')) {
        ReadChar(Lexer);
    }
}

Token NextToken(Lexer* Lexer) {
    Token Token;

    SkipWhitespace(Lexer);

    switch (Lexer->CurrentChar) {
        case '=': {
            Token = NewToken(TOKEN_ASSIGN, "=");
        } break;
        case '+': {
            Token = NewToken(TOKEN_PLUS, "+");
        } break;
        case '-': {
            Token = NewToken(TOKEN_MINUS, "-");
        } break;
        case '!': {
            Token = NewToken(TOKEN_BANG, "!");
        } break;
        case '/': {
            Token = NewToken(TOKEN_SLASH, "/");
        } break;
        case '*': {
            Token = NewToken(TOKEN_ASTERISK, "*");
        } break;
        case '<': {
            Token = NewToken(TOKEN_LT, "<");
        } break;
        case '>': {
            Token = NewToken(TOKEN_GT, ">");
        } break;
        case ';': {
            Token = NewToken(TOKEN_SEMICOLON, ";");
        } break;
        case ',': {
            Token = NewToken(TOKEN_COMMA, ",");
        } break;
        case '(': {
            Token = NewToken(TOKEN_LPAREN, "(");
        } break;
        case ')': {
            Token = NewToken(TOKEN_RPAREN, ")");
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
        } break;
        default: {
            if (IsLetter(Lexer->CurrentChar)) {
                Token.Literal = ReadIdentifier(Lexer);
                Token.TokenType = LookupIdent(Token.Literal);
                return Token;
            } else if (isdigit(Lexer->CurrentChar)) {
                Token.TokenType = TOKEN_INT;
                Token.Literal = ReadNumber(Lexer);
                return Token;
            } else {
                char IllegalChar[2] = "\0";
                IllegalChar[0] = Lexer->CurrentChar;
                Token = NewToken(TOKEN_ILLEGAL, IllegalChar);
            }
        } break;
    }

    ReadChar(Lexer);

    return Token;
}
