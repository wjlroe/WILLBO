CC = gcc
CFLAGS = -g -O0 -Wall -std=c99
DEPS = token/token.h lexer/lexer.h
OBJ = main.o token/token.o lexer/lexer.o
TEST_FILES = $(DEPS) $(OBJ) ../minunit.h ../dbg.h tests.c lexer/lexer_test.c

.PHONEY: clean tests

.DEFAULT_GOAL := tests

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ -I. $<

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ -I. $^

tests.o: $(TEST_FILES)
	$(CC) $(CFLAGS) -o $@ -I. tests.c

tests: tests.o
	@./tests.o

clean:
	rm -rf **/*.o *.o main
