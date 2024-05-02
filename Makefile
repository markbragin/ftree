ifeq ($(origin CC),default)
	CC = gcc
endif

CFLAGS ?= -Wall -Wpedantic -g -ansi -pedantic
LDFLAGS ?=

BINARY ?= ftree
BUILD_DIR ?= build
SOURCE_DIR = src

SOURCES = src/main.c src/tree.c src/dynamic_array.c src/argparser.c
OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)

INCLUDE_DIRS = src
INCLUDE_FLAGS = $(addprefix -I,$(INCLUDE_DIRS))
DEPS := $(OBJECTS:%.o=%.d)

override CFLAGS += $(INCLUDE_FLAGS) -D_DEFAULT_SOURCE


.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJECTS): $(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPS): $(BUILD_DIR)/%.d: %.c
	@mkdir -p $(@D)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@


.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(DEPS) $(BINARY)

.PHONY: run
run: all
	@./$(BINARY)

.PHONY: release
release: CFLAGS := -O2 $(INCLUDE_FLAGS) -NDEBUG
release: all

NODEPS = clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif
