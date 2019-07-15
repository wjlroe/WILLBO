package lexer

import (
	"testing"

	"github.com/wjlroe/WILLBO/token"
)

func TestNextToken(t *testing.T) {
	input := `int intj = 4 + 2 - 3;

	5 < 10 * 2 > 5 != 200;

	string this_is_a_string = "foobar";`

	tests := []struct {
		expectedType    token.TokenType
		expectedLiteral string
	}{
		{token.RESERVED, "int"},
		{token.IDENT, "intj"},
		{token.ASSIGN, "="},
		{token.NUMBER, "4"},
		{token.PLUS, "+"},
		{token.NUMBER, "2"},
		{token.MINUS, "-"},
		{token.NUMBER, "3"},
		{token.SEMICOLON, ";"},
		{token.NUMBER, "5"},
		{token.LT, "<"},
		{token.NUMBER, "10"},
		{token.ASTERISK, "*"},
		{token.NUMBER, "2"},
		{token.GT, ">"},
		{token.NUMBER, "5"},
		{token.NOT_EQ, "!="},
		{token.NUMBER, "200"},
		{token.SEMICOLON, ";"},
		{token.RESERVED, "string"},
		{token.IDENT, "this_is_a_string"},
		{token.ASSIGN, "="},
		{token.STRING, "foobar"},
	}

	l := NewLexer(input)

	for i, tt := range tests {
		tok := l.NextToken()

		if tok.Type != tt.expectedType {
			t.Fatalf("tests[%d] - tokentype wrong. expected=%q, got=%q", i, tt.expectedType, tok.Type)
		}

		if tok.Literal != tt.expectedLiteral {
			t.Fatalf("tests[%d] - literal wrong. expected=%q, got=%q", i, tt.expectedLiteral, tok.Literal)
		}
	}
}
