package token

// TokenType is a type marker for tokens
type TokenType string

// Token types
const (
	Illegal = "ILLEGAL"
	EOF     = "EOF"

	// Identifiers and literals

	Ident  = "IDENT"
	Number = "NUMBER"
	String = "STRING"

	// Operators

	Assign   = "="
	Plus     = "+"
	Minus    = "-"
	Asterisk = "*"
	Bang     = "!"

	Gt = ">"
	Lt = "<"

	Eq    = "=="
	NotEq = "!="

	// Delimiters

	Semicolon = ";"

	Reserved = "RESERVED"
)

// Token represents a single token from the lexer
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
		return Reserved
	}
	return Ident
}

func isReserved(word string) bool {
	if _, ok := reservedWords[word]; ok {
		return true
	}
	return false
}
