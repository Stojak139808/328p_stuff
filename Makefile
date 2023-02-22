# compiler / linker
CC       := avr-gcc
OBJ_COPY := avr-objcopy

# directories
BUILD_DIR := build
SRC_DIR := src
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)

HEX := $(BIN_DIR)/kociara.hex
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# compiler flags
CFLAGS := -mmcu=atmega328 -O
OBJCOPYFLAGS := -O ihex

.PHONY: all clean

all: $(HEX)

$(HEX): $(OBJ) | $(BIN_DIR)
	$(OBJ_COPY) $(OBJCOPYFLAGS) $^ $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -o $@ $(CFLAGS) $< 

$(BIN_DIR) $(OBJ_DIR) $(BUILD_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)