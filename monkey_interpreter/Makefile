CC = gcc
CFLAGS = -g -Og -Wall -std=c99 -fstack-protector-strong
DEPS = token/token.h lexer/lexer.h repl/repl.h ast/ast.h parser/parser.h
OBJ = token/token.o lexer/lexer.o repl/repl.o ast/ast.o parser/parser.o
TEST_FILES = $(DEPS) $(OBJ) ../minunit.h ../dbg.h tests.c lexer/lexer_test.c parser/parser_test.c

VFLAGS  = --quiet
VFLAGS += --tool=memcheck
VFLAGS += --leak-check=full
VFLAGS += --error-exitcode=1
VFLAGS += --track-origins=yes

.PHONEY: clean tests

.DEFAULT_GOAL := tests

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ -I. $<

main.exe: main.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ -I. $^

tests.o: $(TEST_FILES)
	$(CC) $(CFLAGS) -o $@ -I. tests.c

tests: tests.o
	@gdb -q -ex run -ex quit tests.o

memcheck: tests.o
	@valgrind $(VFLAGS) ./tests.o
	@echo "Memory check passed"

repl: main.exe
	@./main.exe

clean:
	rm -rf **/*.o *.o main
