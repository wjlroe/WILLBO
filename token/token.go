package token

// Type is a type marker for tokens
type Type string

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
	Type    Type
	Literal string
}

var reservedWords = map[string]bool{
	"int":    true,
	"string": true,
}

// IdentifierType checks if an identifier is a reserved word or not
func IdentifierType(ident string) Type {
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
