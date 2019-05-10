SHELL := bash

CC := gcc
LD := gcc

CFLAGS := -Wall -Wextra -Werror -MMD -D_GNU_SOURCE
CFLAGS += -O3
LDFLAGS := -lgmp

.PHONY: all
all: validate_challenge lcs35

validate_challenge.o:
	$(CC) $(CFLAGS) -c validate_challenge.c
lcs35.o:
	$(CC) $(CFLAGS) -c lcs35.c

validate_challenge: validate_challenge.o
	$(LD) $(LDFLAGS) $^ -o $@
lcs35: lcs35.o
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f {lcs35,validate_challenge}{,.o,.d,.c~}

-include validate_challenge.d lcs35.d

# @@@ build lcs35_montgomery
# @@@ create an indent target
