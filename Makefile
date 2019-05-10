SHELL := bash

CC := gcc
LD := gcc

CFLAGS := -Wall -Wextra -Werror -MMD -D_GNU_SOURCE
CFLAGS += -O2 -flto
LDFLAGS := -lgmp
LDFLAGS += -O2 -flto

TARGETS := lcs35 validate_challenge
.PHONY: all
all: $(TARGETS)

L_OBJECTS := lcs35.o
VC_OBJECTS := validate_challenge.o
OBJECTS := $(L_OBJECTS) $(VC_OBJECTS)
DEPS := $(patsubst %.o,%.d,$(OBJECTS))
L_SOURCES := $(patsubst %.o,%.c,$(L_OBJECTS))
VC_SOURCES := $(patsubst %.o,%.c,$(VC_OBJECTS))

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

lcs35: $(L_OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@
validate_challenge: $(VC_OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(DEPS)
	rm -f $(TARGETS)

-include $(DEPS)

# @@@ build lcs35_montgomery
# @@@ create an indent target
