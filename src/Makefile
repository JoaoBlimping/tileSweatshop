OBJS = tileSweatshop.cc Tile.cc Tool.cc Tools.cc PaintingContext.cc PaletteContext.cc

OBJ_NAME = tileSweatshop

CC = g++
COMPILER_FLAGS = -std=gnu++11 `pkg-config --cflags gtk+-3.0`
LINKER_FLAGS = `pkg-config --libs gtk+-3.0`



all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
