CC        := gcc

MODE ?= debug

DIR_SB    := ../sb
DIR_TEST  := test
DIR_CORE  := src
DIR_BUILD := build/$(MODE)

SRCS_CORE := $(wildcard $(DIR_CORE)/*.c)
SRCS_TEST := $(wildcard $(DIR_TEST)/*.c)
SRCS_SB   := $(DIR_SB)/sb_int.c $(DIR_SB)/sb_str.c $(DIR_SB)/sb_mem_freelist.c

TARGET      := clox
TARGET_TESTS := $(patsubst $(DIR_TEST)/%.c,$(DIR_BUILD)/$(DIR_TEST)/%_test,$(SRCS_TEST))

# preprocessor
CPPFLAGS  := -I$(DIR_SB) -I$(DIR_CORE)
ifeq ($(MODE),debug)
CPPFLAGS += -DDEBUG_MODE
endif

# fordító
WARN      := -Wall -Wextra -Wpedantic -Werror=return-type -Werror=implicit-function-declaration -Wshadow
CFLAGS    := -std=c17 $(WARN) -g -O0
LDFLAGS   :=
DEPFLAGS  := -MMD -MP
SANFLAGS  := -fsanitize=address,undefined -fno-omit-frame-pointer

ifeq ($(MODE),san)
CFLAGS    += $(SANFLAGS)
LDFLAGS   += $(SANFLAGS)
endif

# path substitution
OBJS      := $(patsubst $(DIR_CORE)/%.c,$(DIR_BUILD)/$(DIR_CORE)/%.o,$(SRCS_CORE)) $(patsubst $(DIR_SB)/%.c,$(DIR_BUILD)/sb/%.o,$(SRCS_SB))
CORE_OBJS := $(filter-out $(DIR_BUILD)/$(DIR_CORE)/main.o,$(OBJS))
TEST_OBJS := $(patsubst $(DIR_TEST)/%.c,$(DIR_BUILD)/$(DIR_TEST)/%.o,$(SRCS_TEST))
# substitution reference
DEPS     := $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

# target: prerequisite
# Plus explicit recipe
# link
$(DIR_BUILD)/$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

san:
	$(MAKE) MODE=san test

$(DIR_BUILD)/$(DIR_TEST)/%_test: $(DIR_BUILD)/$(DIR_TEST)/%.o $(CORE_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Pattern rule
# Fordítás
# Plus explicit recipe
$(DIR_BUILD)/$(DIR_CORE)/%.o: $(DIR_CORE)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(DIR_BUILD)/sb/%.o: $(DIR_SB)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(DIR_BUILD)/$(DIR_TEST)/%.o: $(DIR_TEST)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

run: $(DIR_BUILD)/$(TARGET)
	./$(DIR_BUILD)/$(TARGET)

test: $(TARGET_TESTS)
	@for t in $(TARGET_TESTS); do ./$$t || exit 1; done

-include $(DEPS)

clean:
	rm -rf build

.PHONY: clean run test san

.SECONDARY: $(TEST_OBJS)
