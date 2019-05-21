#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token/token.h"

#ifndef LEXER_H
#define LEXER_H

typedef struct {
    char* Input;
    int Position;      // current position in input (points to current char)
    int ReadPosition;  // current reading position in input (after current char)
    char CurrentChar;  // current char under examination
} Lexer;

void InitLexer(Lexer* Lexer, const char* Input);
Token NextToken(Lexer* Lexer);

#endif
