package lexer

import (
	"github.com/wjlroe/WILLBO/token"
)

// Lexer maintains the state of the lexical analysis stage
type Lexer struct {
	input        string
	position     int  // current position in the input (points to current char)
	readPosition int  // current reading position in input (after current char)
	ch           byte // current char under examination
}

// New returns a Lexer structure for your provided input string
func New(input string) *Lexer {
	l := &Lexer{input: input}
	l.readChar()
	return l
}

func (l *Lexer) readChar() {
	if l.readPosition >= len(l.input) {
		l.ch = 0
	} else {
		l.ch = l.input[l.readPosition]
	}
	l.position = l.readPosition
	l.readPosition++
}

// NextToken returns the next token from the Lexer
func (l *Lexer) NextToken() token.Token {
	var tok token.Token

	l.skipWhitespace()

	switch l.ch {
	case '=':
		if l.peekChar() == '=' {
			ch := l.ch
			l.readChar()
			literal := string(ch) + string(l.ch)
			tok = token.Token{Type: token.Eq, Literal: literal}
		} else {
			tok = newToken(token.Assign, l.ch)
		}
	case '!':
		if l.peekChar() == '=' {
			ch := l.ch
			l.readChar()
			literal := string(ch) + string(l.ch)
			tok = token.Token{Type: token.NotEq, Literal: literal}
		} else {
			tok = newToken(token.Bang, l.ch)
		}
	case '"':
		tok.Type = token.String
		tok.Literal = l.readString()
	case '+':
		tok = newToken(token.Plus, l.ch)
	case '/':
		tok = newToken(token.Slash, l.ch)
	case '-':
		tok = newToken(token.Minus, l.ch)
	case ';':
		tok = newToken(token.Semicolon, l.ch)
	case '<':
		tok = newToken(token.Lt, l.ch)
	case '>':
		tok = newToken(token.Gt, l.ch)
	case '*':
		tok = newToken(token.Asterisk, l.ch)
	case '{':
		tok = newToken(token.LBrace, l.ch)
	case '}':
		tok = newToken(token.RBrace, l.ch)
	case ':':
		tok = newToken(token.Colon, l.ch)
	case ',':
		tok = newToken(token.Comma, l.ch)
	case '[':
		tok = newToken(token.LBracket, l.ch)
	case ']':
		tok = newToken(token.RBracket, l.ch)
	case '(':
		tok = newToken(token.LParen, l.ch)
	case ')':
		tok = newToken(token.RParen, l.ch)
	case '|':
		if l.peekChar() == '|' {
			ch := l.ch
			l.readChar()
			literal := string(ch) + string(l.ch)
			tok = token.Token{Type: token.Or, Literal: literal}
		} else {
			tok = newToken(token.Bar, l.ch)
		}
	case 0:
		tok.Literal = ""
		tok.Type = token.EOF
	default:
		if isLetter(l.ch) {
			tok.Literal = l.readIdentifier()
			tok.Type = token.IdentifierType(tok.Literal)
			return tok
		} else if isDigit(l.ch) {
			tok.Type = token.Number
			tok.Literal = l.readNumber()
			return tok
		} else {
			tok = newToken(token.Illegal, l.ch)
		}
	}

	l.readChar()
	return tok
}

func (l *Lexer) skipWhitespace() {
	for l.ch == ' ' || l.ch == '\t' || l.ch == '\n' || l.ch == '\r' {
		l.readChar()
	}
}

func (l *Lexer) readIdentifier() string {
	position := l.position
	for isLetter(l.ch) || isDigit(l.ch) {
		l.readChar()
	}
	return l.input[position:l.position]
}

func (l *Lexer) readNumber() string {
	position := l.position
	for isDigit(l.ch) {
		l.readChar()
	}
	return l.input[position:l.position]
}

func (l *Lexer) readString() string {
	position := l.position + 1
	for {
		l.readChar()
		if l.ch == '"' || l.ch == 0 {
			break
		}
	}
	return l.input[position:l.position]
}

func (l *Lexer) peekChar() byte {
	if l.readPosition >= len(l.input) {
		return 0
	}
	return l.input[l.readPosition]
}

func isDigit(ch byte) bool {
	return '0' <= ch && ch <= '9'
}

func isLetter(ch byte) bool {
	return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_'
}

func newToken(tokenType token.Type, ch byte) token.Token {
	return token.Token{Type: tokenType, Literal: string(ch)}
}
