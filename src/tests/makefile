# Tools
CC = clang
CA65 = ca65
LD65 = ld65
# Added -I here and dependency tracking flags
CFLAGS = -Wall -Wextra -std=c99 -g -I$(INCLUDE_DIR) -MMD -MP

# Directories
SRC_DIR = ..
INCLUDE_DIR = includes
TEST_DIR = .
BUILD_DIR = build

# C sources
C_SRCS = $(SRC_DIR)/octet.c $(SRC_DIR)/opcode.c $(SRC_DIR)/processor.c
# This helper automatically transforms the paths for you
C_OBJS = $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
APPLE1_SRC = $(TEST_DIR)/apple1.c

# Assembly
ASM_SRC = $(TEST_DIR)/wozmon.s
ASM_OBJ = $(BUILD_DIR)/wozmon.o
ASM_BIN = $(BUILD_DIR)/wozmon.bin
ASM_MAP = $(BUILD_DIR)/wozmon.map
ASM_LST = $(BUILD_DIR)/wozmon.lst
LD65_CFG = $(TEST_DIR)/none.cfg
CA65_FLAGS = -g -l $(ASM_LST)
LD65_FLAGS = -C $(LD65_CFG) -vm -m $(ASM_MAP) -o $(ASM_BIN)

all: $(BUILD_DIR)/apple1 $(ASM_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build C objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build apple1 program
$(BUILD_DIR)/apple1: $(C_OBJS) $(APPLE1_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(C_OBJS) $(APPLE1_SRC) -o $@

# Build assembly object
$(ASM_OBJ): $(ASM_SRC) | $(BUILD_DIR)
	$(CA65) $(CA65_FLAGS) --include-dir $(INCLUDE_DIR) -o $(ASM_OBJ) $(ASM_SRC)

# Build assembly binary
$(ASM_BIN): $(ASM_OBJ) | $(BUILD_DIR)
	$(LD65) $(LD65_FLAGS) $(ASM_OBJ)
	# Removed the RM command so incremental builds work

clean:
	$(RM) -rf $(BUILD_DIR)

# Include generated dependency files
-include $(C_OBJS:.o=.d)

.PHONY: all clean