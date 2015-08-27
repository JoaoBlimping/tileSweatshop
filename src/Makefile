#tileSweatshop's stuff
TSS_OBJS = tileSweatshop.cc Tile.cc Tool.cc Tools.cc PaintingContext.cc \
           PaletteContext.cc HotkeyManager.cc utils.cc
TSS_NAME = tileSweatshop
TSS_CFLAGS = -std=gnu++11 `pkg-config --cflags gtk+-3.0`
TSS_LFLAGS = `pkg-config --libs gtk+-3.0`

#tilesetCreate's stuff
TSC_OBJS = tilesetCreate.cc
TSC_NAME = tilesetCreate
TSC_CFLAGS = -std=gnu++11

#tilesetExport's stuff
TSE_OBJS = tilesetExport.cc lodepng.cc
TSE_NAME = tilesetExport
TSE_CFLAGS = -std=gnu++11

BIN = ../bin

CC = g++


#create tileSweatshop
$(BIN)/$(TSS_NAME): $(TSS_OBJS)
	$(CC) $(TSS_OBJS) $(TSS_CFLAGS) $(TSS_LFLAGS) -o $(BIN)/$(TSS_NAME)

#create tilesetCreate
$(BIN)/$(TSC_NAME): $(TSC_OBJS)
	$(CC) $(TSC_OBJS) $(TSC_CFLAGS) -o $(BIN)/$(TSC_NAME)

#create tilesetCreate
$(BIN)/$(TSE_NAME): $(TSE_OBJS)
	$(CC) $(TSE_OBJS) $(TSE_CFLAGS) -o $(BIN)/$(TSE_NAME)

#create everything
all: $(BIN)/$(TSS_NAME) $(BIN)/$(TSC_NAME) $(BIN)/$(TSE_NAME)
