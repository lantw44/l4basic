CC=cc
AR=ar
RM=rm -f
INSTALL=install -m 644
CFLAGS=-Wall -g -I. -I../l4darr
OBJ=toargv.o qarg.o
LIBFILE=libl4arg.a
DESTDIR=/
PREFIX=/usr/local
LIBDIR=$(DESTDIR)/$(PREFIX)/lib

.PHONY: all clean

all: $(LIBFILE)
$(LIBFILE): $(OBJ)
	$(AR) rcs $(LIBFILE) $(OBJ)
clean:
	$(RM) $(LIBFILE) $(OBJ)
install:
	mkdir -p $(LIBDIR)
	$(INSTALL) $(LIBFILE) $(LIBDIR)
uninstall:
	$(RM) $(LIBDIR)/$(LIBFILE)
