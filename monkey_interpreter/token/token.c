#include "token.h"
#include <string.h>

TokenType LookupIdent(char* Ident) {
    size_t NumKeywords = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);
    for (int i = 0; i < NumKeywords; i++) {
        KeywordMapping Mapping = KEYWORDS[i];
        if ((strlen(Mapping.Literal) == strlen(Ident)) &&
            (strcmp(Mapping.Literal, Ident) == 0)) {
            return Mapping.TokenType;
        }
    }
    return TOKEN_IDENT;
}
