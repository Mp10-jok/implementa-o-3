CC = gcc -16
CFLAGS = -Wall -Wextra -std=c11 -O2
SRC_DIR = src
OBJ_DIR = obj
TARGET = scheduler

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

ifdef DEBUG
CFLAGS += -DDEBUG -g
endif

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
		@mkdir -p $(OBJ_DIR)
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -rf $(OBJ_DIR) $(TARGET) *.out

.PHONY: clean
