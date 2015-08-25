#tileSweatshop's stuff
TSS_OBJS = tileSweatshop.cc Tile.cc Tool.cc Tools.cc PaintingContext.cc PaletteContext.cc HotkeyManager.cc
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

BIN_DIR = ../bin

CC = g++


#create tileSweatshop
$(BIN_DIR)/$(TSS_NAME): $(TSS_OBJS)
	$(CC) $(TSS_OBJS) $(TSS_CFLAGS) $(TSS_LFLAGS) -o $(BIN_DIR)/$(TSS_NAME)

#create tilesetCreate
$(BIN_DIR)/$(TSC_NAME): $(TSC_OBJS)
	$(CC) $(TSC_OBJS) $(TSC_CFLAGS) -o $(BIN_DIR)/$(TSC_NAME)

#create tilesetCreate
$(BIN_DIR)/$(TSE_NAME): $(TSE_OBJS)
	$(CC) $(TSE_OBJS) $(TSE_CFLAGS) -o $(BIN_DIR)/$(TSE_NAME)

#create everything
all: $(BIN_DIR)/$(TSS_NAME) $(BIN_DIR)/$(TSC_NAME) $(BIN_DIR)/$(TSE_NAME)
