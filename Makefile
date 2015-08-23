INSTALL_PATH=/usr/local

all:
	cd src;make all

install: all
	cp bin/tileSweatshop $(INSTALL_PATH)/bin/tileSweatshop
