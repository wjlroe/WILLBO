#include "parser.c"
#include "ast/ast.c"
#include "test_inc.h"

test test_parser_let_statements(TestRun* test_run) {
    char* Input =
        "let x = 5;\n"
        "let y = 10;\n"
        "let foobar = 838383;\n";
    Lexer* Lexer = malloc(sizeof(struct Lexer));
    InitLexer(Lexer, Input);
    Parser* Parser = NewParser(Lexer);
    printf("NewParser is done\n");
    // printf("CurrentToken: %s, PeekToken: %s\n",
    //        Parser->CurrentToken.Literal,
    //        Parser->PeekToken.Literal);
    Program* Program = ParseProgram(Parser);
    mu_assert(Program, "ParseProgram() returned NULL");
    printf("ParseProgram finished\n");
    char* ExpectedIdentifiers[3] = {"x", "y", "foobar"};
    size_t NumExpectedStatements =
        sizeof(ExpectedIdentifiers) / sizeof(ExpectedIdentifiers[0]);
    mu_assert_size_equal(ProgramNumStatements(Program), NumExpectedStatements);
    ProgramStatement* ProgramStatement = Program->FirstStatement;
    for (int i = 0; i < NumExpectedStatements; i++) {
        SyntaxNode* Statement = ProgramStatement->SyntaxNode;
        char* Name = ExpectedIdentifiers[i];

        printf("Looking at statement num: %d\n", i);

        mu_assert_int_equal(Statement->NodeType, NODE_LET_STATEMENT);
        mu_assert_int_equal(Statement->StatementType, STATEMENT);
        mu_assert_str_equal(SyntaxNodeTokenLiteral(Statement), "let");
        mu_assert_str_equal(Statement->LetStatement.Name->Identifier.Value,
                            Name);
        mu_assert_str_equal(
            SyntaxNodeTokenLiteral(Statement->LetStatement.Name), Name);

        ProgramStatement = ProgramStatement->NextStatement;
    }
    FreeProgram(Program);
    FreeParser(Parser);
}
