SHELL := /bin/bash

CC := gcc
LD := gcc

CFLAGS := -Wall -Wextra -Werror -MMD -O3
LDFLAGS := -lgmp

.PHONY: all
all: validate_challenge

validate_challenge.o:
	$(CC) $(CFLAGS) -c validate_challenge.c

validate_challenge: validate_challenge.o
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f validate_challenge{,.o,.d,.c~}

-include validate_challenge.d

# @@@ build lcs35
# @@@ create an indent target
