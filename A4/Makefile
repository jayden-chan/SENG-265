CFLAGS = -Wall -std=c99
PEDANTIC = -Wextra -Wpedantic -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion
DEBUG = -D DEBUG -g

default: build

all: build

build:
	gcc $(CFLAGS) sengencode.c -o sengencode

pedantic:
	gcc $(CFLAGS) $(PEDANTIC) sengencode.c -o sengencode

debug:
	gcc $(CFLAGS) $(DEBUG) sengencode.c -o sengencode

clean:
	rm ./sengencode

.PHONY: build pedantic debug clean
