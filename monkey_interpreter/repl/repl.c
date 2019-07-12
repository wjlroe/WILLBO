#include "repl.h"
#include <stdio.h>
#include "lexer/lexer.h"

#define REPL_BUFFER_SIZE 1024

void StartRepl() {
    char Input[REPL_BUFFER_SIZE] = "\0";
    while (1) {
        printf(PROMPT);

        if (fgets(Input, REPL_BUFFER_SIZE, stdin) == NULL) {
            break;
        }

        Lexer* Lexer = malloc(sizeof(*Lexer));
        InitLexer(Lexer, Input);

        Token Token = NextToken(Lexer);
        while (Token.TokenType != TOKEN_EOF) {
            printf("%s\n", Token.Literal);
            FreeToken(Token);
            Token = NextToken(Lexer);
        }
        FreeToken(Token);
    }
}
