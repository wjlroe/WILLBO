#ifndef PARSER_H
#define PARSER_H

#include "ast/ast.h"
#include "lexer/lexer.h"
#include "token/token.h"

typedef struct Parser {
    Lexer* Lexer;
    Token CurrentToken;
    Token PeekToken;
} Parser;

Parser* NewParser(Lexer* Lexer);
Program* ParseProgram(Parser* Parser);
void FreeParser(Parser* Parser);

#endif
