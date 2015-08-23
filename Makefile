INSTALL_PATH=/usr/local

all:
	cd src;make

install: all
	cp src/tileSweatshop $(INSTALL_PATH)/bin/tileSweatshop
