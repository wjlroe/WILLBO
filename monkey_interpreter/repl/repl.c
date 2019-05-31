#include "repl.h"
#include <stdio.h>
#include "lexer/lexer.h"

#define REPL_BUFFER_SIZE 1024

void StartRepl() {
    char Input[REPL_BUFFER_SIZE] = "\0";
    while (1) {
        printf(PROMPT);
        fgets(Input, REPL_BUFFER_SIZE, stdin);

        Lexer* Lexer = malloc(sizeof(Lexer));
        InitLexer(Lexer, Input);

        Token Token = NextToken(Lexer);
        while (Token.TokenType != TOKEN_EOF) {
            printf("%s\n", Token.Literal);
            Token = NextToken(Lexer);
        }
    }
}
