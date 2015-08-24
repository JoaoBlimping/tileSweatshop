INSTALL_PATH = /usr/local
ASSETS_PATH = /etc/tileSweatshop
SRC = src

all:
	cd $(SRC);make all


install: all
	#get rid of weird assets stuff
	rm -f assets/*~
	rm -f assets/#*#

	#copy the executables
	cp bin/* $(INSTALL_PATH)/bin/$*

	#copy the assets
	mkdir -p $(INSTALL_PATH)$(ASSETS_PATH)
	cp assets/* $(INSTALL_PATH)$(ASSETS_PATH)/$*
