CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2
CFLAGS += $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2) -lm

SRC_DIR = src
LIB3D_DIR = lib3d
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIB3D_SRCS = $(wildcard $(LIB3D_DIR)/src/*.c)
LIB3D_OBJS = $(LIB3D_SRCS:$(LIB3D_DIR)/src/%.c=$(OBJ_DIR)/%.o)

TARGET = 3d_obj_viewer
LIB3D = lib3d/lib3dengine.a

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(LIB3D)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) -L$(LIB3D_DIR) -l3dengine

$(LIB3D): $(LIB3D_OBJS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(LIB3D_DIR)/include -c $< -o $@

$(OBJ_DIR)/%.o: $(LIB3D_DIR)/src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(LIB3D_DIR)/include -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(LIB3D)
