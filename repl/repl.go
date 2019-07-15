package repl

import (
	"bufio"
	"fmt"
	"io"

	"github.com/wjlroe/WILLBO/lexer"
	"github.com/wjlroe/WILLBO/token"
)

const prompt = ">> "

// Start the REPL
func Start(in io.Reader, out io.Writer) {
	scanner := bufio.NewScanner(in)

	for {
		fmt.Printf(prompt)
		scanned := scanner.Scan()
		if !scanned {
			return
		}

		line := scanner.Text()
		l := lexer.NewLexer(line)

		var currentToken = l.NextToken()

		for currentToken.Type != token.EOF {
			fmt.Fprintf(out, "Token Type: %q, Literal: %q\n", currentToken.Type, currentToken.Literal)
			currentToken = l.NextToken()
		}
	}
}
