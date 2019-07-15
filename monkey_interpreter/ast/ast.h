#ifndef AST_H
#define AST_H

#include <stddef.h>
#include "token/token.h"

typedef enum {
    NODE_LET_STATEMENT,
    NODE_IDENTIFIER,
} NodeType;

typedef enum {
    STATEMENT,
    EXPRESSION,
} StatementType;

typedef struct SyntaxNode {
    NodeType NodeType;
    StatementType StatementType;
    struct Token Token;
    union {
        struct {
            struct SyntaxNode* Name;
            struct SyntaxNode* Expression;
        } LetStatement;
        struct {
            char* Value;  // Basically the same as Token.Literal
        } Identifier;
    };
} SyntaxNode;

typedef struct ProgramStatement {
    struct SyntaxNode* SyntaxNode;
    struct ProgramStatement* NextStatement;
} ProgramStatement;

typedef struct Program {
    struct ProgramStatement* FirstStatement;
    struct ProgramStatement* LastStatement;
} Program;

char* SyntaxNodeTokenLiteral(SyntaxNode* Node);
void FreeSyntaxNode(SyntaxNode* Node);
char* ProgramTokenLiteral(Program* Program);
void ProgramAddSyntaxNode(Program* Program, SyntaxNode* SyntaxNode);
size_t ProgramNumStatements(Program* Program);
void FreeProgram(Program* Program);

#endif
