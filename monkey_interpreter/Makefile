CC = gcc
CFLAGS = -Wall
DEPS = token/token.h
OBJ = main.o token/token.o

.PHONEY: clean

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf **/*.o *.o main
