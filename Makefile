
.PHONY: all clean install uninstall deinstall remove
all: 
	$(MAKE) -C src all
clean:
	$(MAKE) -C src clean
install:
	$(MAKE) -C src install
uninstall:
	$(MAKE) -C src uninstall

deinstall: uninstall
remove: uninstall
