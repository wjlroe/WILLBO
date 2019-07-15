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
		expectedType    token.Type
		expectedLiteral string
	}{
		{token.Reserved, "int"},
		{token.Ident, "intj"},
		{token.Assign, "="},
		{token.Number, "4"},
		{token.Plus, "+"},
		{token.Number, "2"},
		{token.Minus, "-"},
		{token.Number, "3"},
		{token.Semicolon, ";"},
		{token.Number, "5"},
		{token.Lt, "<"},
		{token.Number, "10"},
		{token.Asterisk, "*"},
		{token.Number, "2"},
		{token.Gt, ">"},
		{token.Number, "5"},
		{token.NotEq, "!="},
		{token.Number, "200"},
		{token.Semicolon, ";"},
		{token.Reserved, "string"},
		{token.Ident, "this_is_a_string"},
		{token.Assign, "="},
		{token.String, "foobar"},
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
