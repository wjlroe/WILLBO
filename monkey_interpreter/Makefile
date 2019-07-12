CC = gcc
CFLAGS = -g -Og -Wall -Werror -std=c99 -fstack-protector-strong
DEPS = token/token.h lexer/lexer.h repl/repl.h
OBJ = token/token.o lexer/lexer.o repl/repl.o
TEST_FILES = $(DEPS) $(OBJ) ../minunit.h ../dbg.h tests.c lexer/lexer_test.c

.PHONEY: clean tests

.DEFAULT_GOAL := tests

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ -I. $<

main.exe: main.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ -I. $^

tests.o: $(TEST_FILES)
	$(CC) $(CFLAGS) -o $@ -I. tests.c

tests: tests.o
	@./tests.o

repl: main.exe
	@./main.exe

clean:
	rm -rf **/*.o *.o main
