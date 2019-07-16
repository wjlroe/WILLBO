package parser

import (
	"strconv"

	"github.com/pkg/errors"
	"github.com/wjlroe/WILLBO/ast"
	"github.com/wjlroe/WILLBO/lexer"
	"github.com/wjlroe/WILLBO/token"
)

const (
	_ int = iota
	lowest
	equals      // ==
	lessGreater // > or <
	sum         // +
	product     // *
	prefix      // -X or !X
	call        // myFunction(X)
	index       // array[index]
)

var precedences = map[token.Type]int{
	token.Eq:       equals,
	token.NotEq:    equals,
	token.Lt:       lessGreater,
	token.Gt:       lessGreater,
	token.Plus:     sum,
	token.Minus:    sum,
	token.Slash:    product,
	token.Asterisk: product,
	token.LParen:   call,
	token.LBracket: index,
}

type (
	prefixParseFn func() ast.Expression
	infixParseFn  func(ast.Expression) ast.Expression
)

// Parser represents the state of the parser, pulling from the lexer
type Parser struct {
	l *lexer.Lexer

	curToken  token.Token
	peekToken token.Token

	errors []error

	prefixParseFns map[token.Type]prefixParseFn
	infixParseFns  map[token.Type]infixParseFn
}

// New provides an initialized Parser
func New(l *lexer.Lexer) *Parser {
	p := &Parser{
		l:      l,
		errors: []error{},
	}

	p.prefixParseFns = make(map[token.Type]prefixParseFn)
	p.registerPrefix(token.Ident, p.parseIdentifier)
	p.registerPrefix(token.Number, p.parseNumberLiteral)
	p.registerPrefix(token.Bang, p.parsePrefixExpression)
	p.registerPrefix(token.Minus, p.parsePrefixExpression)
	p.registerPrefix(token.LParen, p.parseGroupedExpression)
	p.registerPrefix(token.String, p.parseStringLiteral)
	p.registerPrefix(token.LBracket, p.parseArrayLiteral)
	p.registerPrefix(token.Reserved, p.parseReserved)

	p.infixParseFns = make(map[token.Type]infixParseFn)
	p.registerInfix(token.Plus, p.parseInfixExpression)
	p.registerInfix(token.Minus, p.parseInfixExpression)
	p.registerInfix(token.Slash, p.parseInfixExpression)
	p.registerInfix(token.Asterisk, p.parseInfixExpression)
	p.registerInfix(token.Eq, p.parseInfixExpression)
	p.registerInfix(token.NotEq, p.parseInfixExpression)
	p.registerInfix(token.Lt, p.parseInfixExpression)
	p.registerInfix(token.Gt, p.parseInfixExpression)
	p.registerInfix(token.LParen, p.parseCallExpression)
	p.registerInfix(token.LBracket, p.parseIndexExpression)

	// Read 2 tokens, so curToken and peekToken are both set
	p.nextToken()
	p.nextToken()

	return p
}

func (p *Parser) registerPrefix(tokenType token.Type, fn prefixParseFn) {
	p.prefixParseFns[tokenType] = fn
}

func (p *Parser) registerInfix(tokenType token.Type, fn infixParseFn) {
	p.infixParseFns[tokenType] = fn
}

// ParseProgram parses statements and returns a Program structure
func (p *Parser) ParseProgram() *ast.Program {
	program := &ast.Program{}
	program.Statements = []ast.Statement{}

	for !p.curTokenIsType(token.EOF) {
		stmt := p.parseStatement()
		if stmt != nil {
			program.Statements = append(program.Statements, stmt)
		}
		p.nextToken()
	}

	return program
}

func (p *Parser) parseStatement() ast.Statement {
	if p.curToken.Type == token.Reserved && p.curToken.Literal == "let" {
		return p.parseLetStatement()
	} else if p.curToken.Type == token.Reserved && p.curToken.Literal == "return" {
		return p.parseReturnStatement()
	}
	return p.parseExpressionStatement()
}

func (p *Parser) parseLetStatement() *ast.LetStatement {
	stmt := &ast.LetStatement{Token: p.curToken}

	if !p.expectPeekType(token.Ident) {
		return nil
	}

	stmt.Name = &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}

	p.skipOverType()

	if !p.expectPeekType(token.Assign) {
		return nil
	}

	p.nextToken()

	stmt.Value = p.parseExpression(lowest)

	if p.peekTokenIsType(token.Semicolon) {
		p.nextToken()
	}

	return stmt
}

func (p *Parser) parseReturnStatement() *ast.ReturnStatement {
	stmt := &ast.ReturnStatement{Token: p.curToken}

	p.nextToken()

	stmt.ReturnValue = p.parseExpression(lowest)

	if p.peekTokenIsType(token.Semicolon) {
		p.nextToken()
	}

	return stmt
}

func (p *Parser) parseExpressionStatement() *ast.ExpressionStatement {
	stmt := &ast.ExpressionStatement{Token: p.curToken}

	stmt.Expression = p.parseExpression(lowest)

	if p.peekTokenIsType(token.Semicolon) {
		p.nextToken()
	}

	return stmt
}

func (p *Parser) parseExpression(precedence int) ast.Expression {
	prefix := p.prefixParseFns[p.curToken.Type]
	if prefix == nil {
		prefixError := p.noPrefixParseFnError(p.curToken.Type)
		p.addError(prefixError)
		return nil
	}
	leftExp := prefix()

	for !p.peekTokenIsType(token.Semicolon) && precedence < p.peekPrecedence() {
		infix := p.infixParseFns[p.peekToken.Type]
		if infix == nil {
			return leftExp
		}

		p.nextToken()

		leftExp = infix(leftExp)
	}

	return leftExp
}

func (p *Parser) parseIdentifier() ast.Expression {
	return &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}
}

func (p *Parser) parseNumberLiteral() ast.Expression {
	lit := &ast.NumberLiteral{Token: p.curToken}

	value, err := strconv.ParseInt(p.curToken.Literal, 0, 32)
	if err != nil {
		msg := errors.Errorf("could not parse %q as integer", p.curToken.Literal)
		p.addError(msg)
		return nil
	}

	lit.Value = int32(value)

	return lit
}

func (p *Parser) parseStringLiteral() ast.Expression {
	return &ast.StringLiteral{Token: p.curToken, Value: p.curToken.Literal}
}

func (p *Parser) parseArrayLiteral() ast.Expression {
	array := &ast.ArrayLiteral{Token: p.curToken}

	array.Elements = p.parseExpressionList(token.RBracket)

	return array
}

func (p *Parser) parseReserved() ast.Expression {
	switch p.curToken.Literal {
	case "fn":
		return p.parseFunctionLiteral()
	case "true":
		return p.parseBoolean()
	case "false":
		return p.parseBoolean()
	case "bool":
		return p.parseTypeExpression()
	case "if":
		return p.parseIfExpression()
	}
	msg := errors.Errorf("unknown reserved word: %q", p.curToken.Literal)
	p.addError(msg)
	return nil
}

func (p *Parser) parseBoolean() ast.Expression {
	boolVal := p.curToken.Literal == "true"
	return &ast.Boolean{Token: p.curToken, Value: boolVal}
}

func (p *Parser) parseGroupedExpression() ast.Expression {
	p.nextToken()

	exp := p.parseExpression(lowest)

	if !p.expectPeekType(token.RParen) {
		return nil
	}

	return exp
}

func (p *Parser) parseIfExpression() ast.Expression {
	expression := &ast.IfExpression{Token: p.curToken}

	if !p.expectPeekType(token.LParen) {
		return nil
	}

	p.nextToken()
	expression.Condition = p.parseExpression(lowest)

	if !p.expectPeekType(token.RParen) {
		return nil
	}

	if !p.expectPeekType(token.LBrace) {
		return nil
	}

	expression.Consequence = p.parseBlockStatement()

	if p.peekTokenIsType(token.Reserved) && p.peekTokenIsLiteral("else") {
		p.nextToken()

		if !p.expectPeekType(token.LBrace) {
			return nil
		}

		expression.Alternative = p.parseBlockStatement()
	}

	return expression
}

func (p *Parser) parseFunctionLiteral() ast.Expression {
	lit := &ast.FunctionLiteral{Token: p.curToken}

	if !p.expectPeekType(token.LParen) {
		return nil
	}

	lit.Parameters = p.parseFunctionParameters()

	if !p.expectPeekType(token.LBrace) {
		return nil
	}

	lit.Body = p.parseBlockStatement()

	return lit
}

func (p *Parser) parseFunctionParameters() []*ast.Identifier {
	identifiers := []*ast.Identifier{}

	if p.peekTokenIsType(token.RParen) {
		p.nextToken()
		return identifiers
	}

	p.nextToken()

	ident := &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}
	identifiers = append(identifiers, ident)

	p.skipOverType()

	for p.peekTokenIsType(token.Comma) {
		p.nextToken()
		p.nextToken()
		ident := &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}
		identifiers = append(identifiers, ident)
		p.skipOverType()
	}

	if !p.expectPeekType(token.RParen) {
		return nil
	}

	p.skipOverType()

	return identifiers
}

func (p *Parser) parseBlockStatement() *ast.BlockStatement {
	block := &ast.BlockStatement{Token: p.curToken}
	block.Statements = []ast.Statement{}

	p.nextToken()

	for !p.curTokenIsType(token.RBrace) && !p.curTokenIsType(token.EOF) {
		stmt := p.parseStatement()
		if stmt != nil {
			block.Statements = append(block.Statements, stmt)
		}
		p.nextToken()
	}

	return block
}

func (p *Parser) parsePrefixExpression() ast.Expression {
	expression := &ast.PrefixExpression{
		Token:    p.curToken,
		Operator: p.curToken.Literal,
	}

	p.nextToken()

	expression.Right = p.parseExpression(prefix)

	return expression
}

func (p *Parser) parseInfixExpression(left ast.Expression) ast.Expression {
	expression := &ast.InfixExpression{
		Token:    p.curToken,
		Operator: p.curToken.Literal,
		Left:     left,
	}

	precedence := p.curPrecedence()
	p.nextToken()
	expression.Right = p.parseExpression(precedence)

	return expression
}

func (p *Parser) parseCallExpression(function ast.Expression) ast.Expression {
	exp := &ast.CallExpression{Token: p.curToken, Function: function}
	exp.Arguments = p.parseExpressionList(token.RParen)
	return exp
}

func (p *Parser) parseExpressionList(end token.Type) []ast.Expression {
	list := []ast.Expression{}

	if p.peekTokenIsType(end) {
		p.nextToken()
		return list
	}

	p.nextToken()
	list = append(list, p.parseExpression(lowest))

	for p.peekTokenIsType(token.Comma) {
		p.nextToken()
		p.nextToken()
		list = append(list, p.parseExpression(lowest))
	}

	if !p.expectPeekType(end) {
		return nil
	}

	return list
}

func (p *Parser) parseIndexExpression(left ast.Expression) ast.Expression {
	exp := &ast.IndexExpression{Token: p.curToken, Left: left}

	p.nextToken()
	exp.Index = p.parseExpression(lowest)

	if !p.expectPeekType(token.RBracket) {
		return nil
	}

	return exp
}

func (p *Parser) parseTypeExpression() ast.Expression {
	return nil
}

// curToken/peekToken helper functions

func (p *Parser) nextToken() {
	p.curToken = p.peekToken
	p.peekToken = p.l.NextToken()
}

func (p *Parser) curTokenIsType(t token.Type) bool {
	return p.curToken.Type == t
}

func (p *Parser) curTokenIsLiteral(l string) bool {
	return p.curToken.Literal == l
}

func (p *Parser) peekTokenIsType(t token.Type) bool {
	return p.peekToken.Type == t
}

func (p *Parser) peekTokenIsLiteral(l string) bool {
	return p.peekToken.Literal == l
}

func (p *Parser) expectPeekType(t token.Type) bool {
	if p.peekTokenIsType(t) {
		p.nextToken()
		return true
	}
	p.peekErrorType(t)
	return false
}

func (p *Parser) skipOverType() bool {
	// FIXME: hack moving past the type annotation...
	if p.peekTokenIsType(token.Colon) {
		p.nextToken() // skip the ':'
		if p.peekTokenIsLiteral("i32") || p.peekTokenIsLiteral("string") || p.peekTokenIsLiteral("bool") {
			p.nextToken() // skip over the type name
		}
		return true
	}
	return false
}

// Precedence helper functions

func (p *Parser) curPrecedence() int {
	if p, ok := precedences[p.curToken.Type]; ok {
		return p
	}

	return lowest
}

func (p *Parser) peekPrecedence() int {
	if p, ok := precedences[p.peekToken.Type]; ok {
		return p
	}

	return lowest
}

// Errors helper functions

func (p *Parser) addError(err error) {
	p.errors = append(p.errors, err)
}

func (p *Parser) peekErrorType(t token.Type) {
	msg := errors.Errorf("expected next token to be '%s', got '%s' (literal: '%s') instead",
		t, p.peekToken.Type, p.peekToken.Literal)
	p.addError(msg)
}

func (p *Parser) peekErrorLiteral(l string) {
	msg := errors.Errorf("expected next token literal to be '%s', got '%s' instead",
		l, p.peekToken.Literal)
	p.addError(msg)
}

func (p *Parser) noPrefixParseFnError(t token.Type) error {
	return errors.Errorf("no prefix parse function for '%s' found", t)
}

// Errors provides all the parsing errors that occured during parsing
func (p *Parser) Errors() []error {
	return p.errors
}
