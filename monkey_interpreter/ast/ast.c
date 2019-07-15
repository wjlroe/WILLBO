#include "ast.h"
#include <stdlib.h>

char* SyntaxNodeTokenLiteral(SyntaxNode* Node) {
    return Node->Token.Literal;
}

void FreeSyntaxNode(SyntaxNode* Node) {
    if (Node->NodeType == NODE_LET_STATEMENT) {
        FreeSyntaxNode(Node->LetStatement.Name);
        FreeSyntaxNode(Node->LetStatement.Expression);
    }
    free(Node);
}

char* ProgramTokenLiteral(Program* Program) {
    if (Program->FirstStatement != NULL) {
        return SyntaxNodeTokenLiteral(Program->FirstStatement->SyntaxNode);
    } else {
        return "";
    }
}

void ProgramAddSyntaxNode(Program* Program, SyntaxNode* SyntaxNode) {
    ProgramStatement* NewStatement = malloc(sizeof(struct ProgramStatement));
    NewStatement->NextStatement = NULL;
    NewStatement->SyntaxNode = SyntaxNode;
    if (Program->LastStatement != NULL) {
        Program->LastStatement->NextStatement = NewStatement;
    }
    Program->LastStatement = NewStatement;
    if (Program->FirstStatement == NULL) {
        Program->FirstStatement = NewStatement;
    }
}

size_t ProgramNumStatements(Program* Program) {
    if (Program->FirstStatement == NULL) {
        return 0;
    }
    size_t NumStatements = 0;
    ProgramStatement* Statement = Program->FirstStatement;
    while (Statement != NULL) {
        NumStatements++;
        Statement = Statement->NextStatement;
    }
    return NumStatements;
}

void FreeProgram(Program* Program) {
    ProgramStatement* Statement = Program->FirstStatement;
    while (Statement != NULL) {
        FreeSyntaxNode(Statement->SyntaxNode);
        Statement = Statement->NextStatement;
    }
}
