SHELL := bash

CC := gcc
LD := gcc

C_SRCDIR := src/main/c
TARGETDIR := target

COMMON_FLAGS := -O2 -flto -g
CFLAGS := -Wall -Wextra -Werror -MMD -D_GNU_SOURCE -ansi -pedantic
CFLAGS += $(COMMON_FLAGS)
LDFLAGS := -lgmp
LDFLAGS += $(COMMON_FLAGS)

TARGETS := lcs35 lcs35m validate_challenge

ifeq ($(RUN_EXAMPLE),1)
CFLAGS += -D_RUN_EXAMPLE
else ifeq ($(RUN_FAKE),1)
CFLAGS += -D_RUN_FAKE
else ifeq ($(RUN_FULL),1)
CFLAGS += -D_RUN_FULL
endif

L_OBJECTS := lcs35.o
LM_OBJECTS := lcs35m.o
VC_OBJECTS := validate_challenge.o parse_challenge_message.o
COMMON_OBJECTS := print_challenge_message.o

L_SOURCES := $(patsubst $(TARGETDIR)/%.o,$(C_SRCDIR)/%.c,$(L_OBJECTS))
LM_SOURCES := $(patsubst $(TARGETDIR)/%.o,$(C_SRCDIR)/%.c,$(LM_OBJECTS))
VC_SOURCES := $(patsubst $(TARGETDIR)/%.o,$(C_SRCDIR)/%.c,$(VC_OBJECTS))
COMMON_SOURCES := $(patsubst $(TARGETDIR)/%.o,$(C_SRCDIR)/%.c,$(COMMON_OBJECTS))
HEADERS := lcs35.h challenge.h print_challenge_message.h parse_challenge_message.h
SOURCES := $(addprefix $(C_SRCDIR)/,$(L_SOURCES) $(LM_SOURCES) $(VC_SOURCES) $(COMMON_SOURCES) $(HEADERS))

L_OBJECTS := $(addprefix $(TARGETDIR)/,$(L_OBJECTS))
LM_OBJECTS := $(addprefix $(TARGETDIR)/,$(LM_OBJECTS))
VC_OBJECTS := $(addprefix $(TARGETDIR)/,$(VC_OBJECTS))
COMMON_OBJECTS := $(addprefix $(TARGETDIR)/,$(COMMON_OBJECTS))

OBJECTS := $(L_OBJECTS) $(LM_OBJECTS) $(VC_OBJECTS) $(COMMON_OBJECTS)
DEPS := $(patsubst %.o,%.d,$(OBJECTS))

.PHONY: all
all: $(TARGETS)

$(TARGETDIR):
	mkdir -p $(TARGETDIR)
$(OBJECTS): | $(TARGETDIR)

$(OBJECTS): $(TARGETDIR)/%.o: $(C_SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

lcs35: $(L_OBJECTS) $(COMMON_OBJECTS)
	$(LD) $^ $(LDFLAGS) -o $@
lcs35m: $(LM_OBJECTS) $(COMMON_OBJECTS)
	$(LD) $^ $(LDFLAGS) -o $@
validate_challenge: $(VC_OBJECTS) $(COMMON_OBJECTS)
	$(LD) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(DEPS)
	rm -f $(TARGETS)
	test ! -e $(TARGETDIR) || rmdir $(TARGETDIR)

.PHONY: indent
indent:
	for f in \
		$(SOURCES) \
	; do \
		indent -gnu -nut -l120 $$f -o _indent.tmp; \
		diff --brief $$f _indent.tmp >/dev/null 2>&1 || mv -f _indent.tmp $$f; \
		rm -f _indent.tmp; \
	done

-include $(DEPS)
