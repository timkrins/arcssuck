PROGRAM = generate

SOURCES = $(PROGRAM).c
CC = $(ARCH)gcc

.PHONY: all program clean

all:
	$(CC) $(SOURCES) -o $(PROGRAM).exe
	size $(PROGRAM).exe

clean:
	rm -rf $(PROGRAM).exe