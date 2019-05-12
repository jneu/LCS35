SHELL := bash

CC := gcc
LD := gcc

CFLAGS := -Wall -Wextra -Werror -MMD -D_GNU_SOURCE -g
CFLAGS += -O2 -flto
LDFLAGS := -g
LDFLAGS += -lgmp
LDFLAGS += -O2 -flto

TARGETS := lcs35 lcs35m validate_challenge

L_OBJECTS := lcs35.o
LM_OBJECTS := lcs35m.o
VC_OBJECTS := validate_challenge.o
COMMON_OBJECTS := print_challenge_message.o

L_SOURCES := $(patsubst %.o,%.c,$(L_OBJECTS))
LM_SOURCES := $(patsubst %.o,%.c,$(LM_OBJECTS))
VC_SOURCES := $(patsubst %.o,%.c,$(VC_OBJECTS))
COMMON_SOURCES := $(patsubst %.o,%.c,$(COMMON_OBJECTS))
HEADERS := lcs35.h challenge.h print_challenge_message.h
SOURCES := $(L_SOURCES) $(LM_SOURCES) $(VC_SOURCES) $(COMMON_SOURCES) $(HEADERS)

OBJECTS := $(L_OBJECTS) $(LM_OBJECTS) $(VC_OBJECTS) $(COMMON_OBJECTS)
DEPS := $(patsubst %.o,%.d,$(OBJECTS))

.PHONY: all
all: $(TARGETS)

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

lcs35: $(L_OBJECTS) $(COMMON_OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@
lcs35m: $(LM_OBJECTS) $(COMMON_OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@
validate_challenge: $(VC_OBJECTS) $(COMMON_OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(DEPS)
	rm -f $(TARGETS)

.PHONY: indent
indent:
	for f in $(SOURCES); do \
		indent -gnu -nut -l120 $$f -o _indent.tmp; \
		diff --brief $$f _indent.tmp >/dev/null 2>&1 || mv -f _indent.tmp $$f; \
		rm -f _indent.tmp; \
	done

-include $(DEPS)
