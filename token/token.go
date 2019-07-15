package token

type TokenType string

const (
	ILLEGAL = "ILLEGAL"
	EOF     = "EOF"

	// Identifiers and literals
	IDENT  = "IDENT"
	NUMBER = "NUMBER"
	STRING = "STRING"

	// Operators
	ASSIGN   = "="
	PLUS     = "+"
	MINUS    = "-"
	ASTERISK = "*"

	GT = ">"
	LT = "<"

	EQ = "=="

	// Delimiters
	SEMICOLON = ";"

	RESERVED = "RESERVED"
)

type Token struct {
	Type    TokenType
	Literal string
}

var reservedWords = map[string]bool{
	"int":    true,
	"string": true,
}

// IdentifierType checks if an identifier is a reserved word or not
func IdentifierType(ident string) TokenType {
	if isReserved(ident) {
		return RESERVED
	}
	return IDENT
}

func isReserved(word string) bool {
	if _, ok := reservedWords[word]; ok {
		return true
	}
	return false
}
