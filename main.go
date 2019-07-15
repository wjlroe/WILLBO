package main

import (
	"fmt"
	"os"

	"github.com/wjlroe/WILLBO/repl"
)

func main() {
	fmt.Println("Welcome to the WILLBO REPL")
	repl.Start(os.Stdin, os.Stdout)
}
