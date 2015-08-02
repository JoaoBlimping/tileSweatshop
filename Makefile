OBJS = main.cc Tile.cc

CC = g++

COMPILER_FLAGS = `pkg-config --cflags gtk+-3.0`

LINKER_FLAGS = `pkg-config --libs gtk+-3.0`

OBJ_NAME = tileSweatshop

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
