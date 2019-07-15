#include "parser.h"
#include <stdbool.h>
#include "ast/ast.h"
#include "lexer/lexer.h"

void ParseNextToken(Parser* Parser) {
    Parser->CurrentToken = Parser->PeekToken;
    Parser->PeekToken = NextToken(Parser->Lexer);
    printf("ParseNextToken. CurrentToken: %s, PeekToken: %s\n",
           Parser->CurrentToken.Literal,
           Parser->PeekToken.Literal);
}

bool CurrentTokenIs(Parser* Parser, TokenType TokenType) {
    return (Parser->CurrentToken.TokenType == TokenType);
}

bool PeekTokenIs(Parser* Parser, TokenType TokenType) {
    return (Parser->PeekToken.TokenType == TokenType);
}

bool ExpectPeek(Parser* Parser, TokenType TokenType) {
    if (PeekTokenIs(Parser, TokenType)) {
        ParseNextToken(Parser);
        return true;
    } else {
        return false;
    }
}

Parser* NewParser(Lexer* Lexer) {
    Parser* Parser = malloc(sizeof(struct Parser));
    Parser->PeekToken = ZeroToken();
    Parser->CurrentToken = ZeroToken();
    Parser->Lexer = Lexer;

    ParseNextToken(Parser);
    ParseNextToken(Parser);

    return Parser;
}

SyntaxNode* ParseLetStatement(Parser* Parser) {
    SyntaxNode* Statement = malloc(sizeof(struct SyntaxNode));
    Statement->NodeType = NODE_LET_STATEMENT;
    Statement->StatementType = STATEMENT;
    Statement->Token = Parser->CurrentToken;
    printf("let statement token type: %d\n", Statement->Token.TokenType);
    printf("let statement token literal: %s\n", Statement->Token.Literal);

    if (!ExpectPeek(Parser, TOKEN_IDENT)) {
        free(Statement);
        return NULL;
    }

    SyntaxNode* Identifier = malloc(sizeof(struct SyntaxNode));
    Identifier->NodeType = NODE_IDENTIFIER;
    Identifier->Token = Parser->CurrentToken;
    Identifier->Identifier.Value = Parser->CurrentToken.Literal;
    Statement->LetStatement.Name = Identifier;

    if (!ExpectPeek(Parser, TOKEN_ASSIGN)) {
        free(Identifier);
        free(Statement);
        return NULL;
    }

    while (!CurrentTokenIs(Parser, TOKEN_SEMICOLON)) {
        ParseNextToken(Parser);
    }

    return Statement;
}

SyntaxNode* ParseStatement(Parser* Parser) {
    switch (Parser->CurrentToken.TokenType) {
        case TOKEN_LET: {
            return ParseLetStatement(Parser);
        } break;
        default: { return NULL; } break;
    }
}

Program* ParseProgram(Parser* Parser) {
    Program* Program = malloc(sizeof(struct Program));
    Program->FirstStatement = NULL;
    Program->LastStatement = NULL;

    while (Parser->CurrentToken.TokenType != TOKEN_EOF) {
        SyntaxNode* Statement = ParseStatement(Parser);
        if (Statement != NULL) {
            printf("Adding new SyntaxNode!\n");
            ProgramAddSyntaxNode(Program, Statement);
        }
        ParseNextToken(Parser);
    }

    return Program;
}

void FreeParser(Parser* Parser) {
    FreeLexer(Parser->Lexer);
    FreeToken(Parser->CurrentToken);
    FreeToken(Parser->PeekToken);
    free(Parser);
}
