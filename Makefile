SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE_DIR := include

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

CFLAGS := -Wextra -Werror -Wall -Wimplicit -pedantic -Wreturn-type -Wformat -Wmissing-prototypes -Wstrict-prototypes -std=c89 -I$(INCLUDE_DIR) -g -O3

TARGET := $(BIN_DIR)/libquant_test

all: $(TARGET)

# build
$(TARGET): $(OBJS) | $(BIN_DIR)
	gcc $(OBJS) -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@

# create directories if missing
$(BIN_DIR) $(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

