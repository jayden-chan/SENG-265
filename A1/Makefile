CFLAGS = -Wall -ansi -std=c99

default: build

all: build run

build:
	gcc $(CFLAGS) sengfmt.c -o sengfmt

pedantic:
	gcc $(CFLAGS) -Wextra sengfmt.c -o sengfmt

debug:
	gcc $(CFLAGS) -g sengfmt.c -o sengfmt

clean:
	rm ./sengfmt

.PHONY: build run pedantic debug clean
