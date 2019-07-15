package token

import (
	"sort"
)

type TokenType string

const (
	ILLEGAL = "ILLEGAL"
	EOF     = "EOF"

	// Identifiers and literals
	IDENT  = "IDENT"
	NUMBER = "NUMBER"
	STRING = "STRING"

	// Operators
	ASSIGN = "="
	PLUS   = "+"

	EQ = "=="

	// Delimiters
	SEMICOLON = ";"

	RESERVED = "RESERVED"
)

type Token struct {
	Type    TokenType
	Literal string
}

var reservedWords = []string{
	"int",
}

func init() {
	sort.Strings(reservedWords)
}

// IdentifierType checks if an identifier is a reserved word or not
func IdentifierType(ident string) TokenType {
	if sort.SearchStrings(reservedWords, ident) < len(reservedWords) {
		return RESERVED
	} else {
		return IDENT
	}
}
