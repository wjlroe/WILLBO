#include "token.h"
#include <stdlib.h>
#include <string.h>

const KeywordMapping* LookupKeyword(char* Ident) {
    size_t NumKeywords = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);
    for (size_t i = 0; i < NumKeywords; i++) {
        const KeywordMapping* Mapping = &KEYWORDS[i];
        if ((strlen(Mapping->Literal) == strlen(Ident)) &&
            (strcmp(Mapping->Literal, Ident) == 0)) {
            return Mapping;
        }
    }
    return NULL;
}

void FreeToken(Token Token) {
    switch (Token.TokenType) {
        case TOKEN_IDENT:
        case TOKEN_INT: {
            free(Token.Literal);
        } break;
        default:
            break;
    }
}

Token ZeroToken() {
    Token Token = {};
    Token.Literal = "";
    Token.TokenType = TOKEN_ILLEGAL;
    return Token;
}
