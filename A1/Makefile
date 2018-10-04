CFLAGS = -Wall -std=c99
PEDANTIC = -Wextra -Wpedantic -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion

default: build

all: build run

build:
	gcc $(CFLAGS) sengfmt.c -o sengfmt

pedantic:
	gcc $(CFLAGS) $(PEDANTIC) sengfmt.c -o sengfmt

debug:
	gcc $(CFLAGS) -g sengfmt.c -o sengfmt

clean:
	rm ./sengfmt

.PHONY: build run pedantic debug clean
