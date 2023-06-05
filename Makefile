CC = gcc
CFLAGS = -Wall -g
DEPS = main.c
OBJ = main.o
BIN = main
FILE = test1

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean run

clean:
	rm -f $(OBJ) $(BIN)

run: $(BIN)
	./$(BIN) $(FILE)

help: 
	@echo "To run this file you ./main networkFile"