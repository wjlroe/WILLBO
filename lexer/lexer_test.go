package lexer

import (
	"testing"

	"github.com/wjlroe/WILLBO/token"
)

func TestNextToken(t *testing.T) {
	input := `let intj : i32 = 4 + 2 - 3 / 2;

	5 < 10 * 2 > 5 != 200;

	let this_is_a_string : string = "foobar";

	struct Position {
		x: i32,
		y: i32,
	}

	struct Entity {
		position: Position,
	}

	struct State {
		entities: []Entity,
	}

	pub fn update(dt: i32) {
		animate_with_dt(dt);
	}

	pub fn render() {
		for entity in entities {
			draw(entity);
		}
	}

	fn main() : i32 {
		writeln("Hello, World");

		let result : bool = false || true;
		let exit_code = if result {
			0
		} else {
			1
		};
		return exit_code;
	}`

	tests := []struct {
		expectedType    token.Type
		expectedLiteral string
	}{
		{token.Reserved, "let"},
		{token.Ident, "intj"},
		{token.Colon, ":"},
		{token.Reserved, "i32"},
		{token.Assign, "="},
		{token.Number, "4"},
		{token.Plus, "+"},
		{token.Number, "2"},
		{token.Minus, "-"},
		{token.Number, "3"},
		{token.Slash, "/"},
		{token.Number, "2"},
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
		{token.Reserved, "let"},
		{token.Ident, "this_is_a_string"},
		{token.Colon, ":"},
		{token.Reserved, "string"},
		{token.Assign, "="},
		{token.String, "foobar"},
		{token.Semicolon, ";"},
		{token.Reserved, "struct"},
		{token.Ident, "Position"},
		{token.LBrace, "{"},
		{token.Ident, "x"},
		{token.Colon, ":"},
		{token.Reserved, "i32"},
		{token.Comma, ","},
		{token.Ident, "y"},
		{token.Colon, ":"},
		{token.Reserved, "i32"},
		{token.Comma, ","},
		{token.RBrace, "}"},
		{token.Reserved, "struct"},
		{token.Ident, "Entity"},
		{token.LBrace, "{"},
		{token.Ident, "position"},
		{token.Colon, ":"},
		{token.Ident, "Position"},
		{token.Comma, ","},
		{token.RBrace, "}"},
		{token.Reserved, "struct"},
		{token.Ident, "State"},
		{token.LBrace, "{"},
		{token.Ident, "entities"},
		{token.Colon, ":"},
		{token.LBracket, "["},
		{token.RBracket, "]"},
		{token.Ident, "Entity"},
		{token.Comma, ","},
		{token.RBrace, "}"},
		{token.Reserved, "pub"},
		{token.Reserved, "fn"},
		{token.Ident, "update"},
		{token.LParen, "("},
		{token.Ident, "dt"},
		{token.Colon, ":"},
		{token.Reserved, "i32"},
		{token.RParen, ")"},
		{token.LBrace, "{"},
		{token.Ident, "animate_with_dt"},
		{token.LParen, "("},
		{token.Ident, "dt"},
		{token.RParen, ")"},
		{token.Semicolon, ";"},
		{token.RBrace, "}"},
		{token.Reserved, "pub"},
		{token.Reserved, "fn"},
		{token.Ident, "render"},
		{token.LParen, "("},
		{token.RParen, ")"},
		{token.LBrace, "{"},
		{token.Reserved, "for"},
		{token.Ident, "entity"},
		{token.Reserved, "in"},
		{token.Ident, "entities"},
		{token.LBrace, "{"},
		{token.Ident, "draw"},
		{token.LParen, "("},
		{token.Ident, "entity"},
		{token.RParen, ")"},
		{token.Semicolon, ";"},
		{token.RBrace, "}"},
		{token.RBrace, "}"},
		{token.Reserved, "fn"},
		{token.Ident, "main"},
		{token.LParen, "("},
		{token.RParen, ")"},
		{token.Colon, ":"},
		{token.Reserved, "i32"},
		{token.LBrace, "{"},
		{token.Ident, "writeln"},
		{token.LParen, "("},
		{token.String, "Hello, World"},
		{token.RParen, ")"},
		{token.Semicolon, ";"},
		{token.Reserved, "let"},
		{token.Ident, "result"},
		{token.Colon, ":"},
		{token.Reserved, "bool"},
		{token.Assign, "="},
		{token.Reserved, "false"},
		{token.Or, "||"},
		{token.Reserved, "true"},
		{token.Semicolon, ";"},
		{token.Reserved, "let"},
		{token.Ident, "exit_code"},
		{token.Assign, "="},
		{token.Reserved, "if"},
		{token.Ident, "result"},
		{token.LBrace, "{"},
		{token.Number, "0"},
		{token.RBrace, "}"},
		{token.Reserved, "else"},
		{token.LBrace, "{"},
		{token.Number, "1"},
		{token.RBrace, "}"},
		{token.Semicolon, ";"},
		{token.Reserved, "return"},
		{token.Ident, "exit_code"},
		{token.Semicolon, ";"},
		{token.RBrace, "}"},
	}

	l := New(input)

	for i, tt := range tests {
		tok := l.NextToken()

		if tok.Type != tt.expectedType {
			t.Fatalf("tests[%d] - tokentype wrong. expected=%q, got=%q (literal: %q)",
				i, tt.expectedType, tok.Type, tok.Literal)
		}

		if tok.Literal != tt.expectedLiteral {
			t.Fatalf("tests[%d] - literal wrong. expected=%q, got=%q",
				i, tt.expectedLiteral, tok.Literal)
		}
	}

	eof := l.NextToken()
	if eof.Type != token.EOF {
		t.Fatalf("Last token - type wrong. expected=%q, got=%q",
			token.EOF, eof.Type)
	}
}
