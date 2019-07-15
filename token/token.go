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
	Slash    = "/"
	Or       = "||"

	Gt = ">"
	Lt = "<"

	Eq    = "=="
	NotEq = "!="

	// Delimiters

	Semicolon = ";"
	Colon     = ":"
	Comma     = ","
	Bar       = "|"

	LBrace   = "{"
	RBrace   = "}"
	LBracket = "["
	RBracket = "]"
	LParen   = "("
	RParen   = ")"

	Reserved = "RESERVED"
)

// Token represents a single token from the lexer
type Token struct {
	Type    Type
	Literal string
}

var reservedWords = map[string]bool{
	"i32":    true,
	"bool":   true,
	"string": true,
	"struct": true,
	"pub":    true,
	"fn":     true,
	"for":    true,
	"in":     true,
	"let":    true,
	"false":  true,
	"true":   true,
	"if":     true,
	"else":   true,
	"return": true,
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
