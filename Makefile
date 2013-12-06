# Programs
HOST=
PROG_CC=     cc
PROG_AR=     ar
PROG_RANLIB= ranlib
CC=          $(HOST)$(PROG_CC)
AR=          $(HOST)$(PROG_AR)
RANLIB=      $(HOST)$(PROG_RANLIB)
RM=          rm -f
MKDIR=       mkdir
MKDIR_P=     $(MKDIR) -p
INSTALL=     install -c -m 644

# User flags
CFLAGS=      -g -O2 -pipe
LDFLAGS=

# Internal flags
L4B_CFLAGS=  -Wall -I. $(CFLAGS)
L4B_LDFLAGS= $(LDFLAGS)

# Installation
DESTDIR=
PREFIX=      /usr/local
LIBDIR=      $(DESTDIR)$(PREFIX)/lib
INCLUDEDIR=  $(DESTDIR)$(PREFIX)/include

# Tasks definition
lib_LIBRARIES=         libl4basic.a
libl4basic_a_OBJECTS=  l4array.o l4array2.o l4list.o l4arg.o
libl4basic_a_HEADERS=  $(libl4basic_a_OBJECTS:.o=.h)

# Build dependencies
l4array_o_DEPENDS=
l4array2_o_DEPENDS=
l4list_o_DEPENDS=
l4arg_o_DEPENDS=       l4array.o

.POSIX:
.PHONY: all clean install install-HEADERS install-LIB \
		uninstall deinstall remove
.SUFFIXES: .c.o
.c.o:
	$(CC) $(L4B_CFLAGS) -c $< -o $@

all: $(lib_LIBRARIES)
libl4basic.a: $(libl4basic_a_OBJECTS)
	$(AR) rcs $@ $(libl4basic_a_OBJECTS)
	$(RANLIB) $@
l4array.o: l4array.c l4array.h $(l4array_o_DEPENDS) 
l4array2.o: l4array2.c l4array2.h $(l4array2_o_DEPENDS)
l4list.o: l4list.c l4list.h $(l4list_o_DEPENDS)
l4arg.o: l4arg.c l4arg.h $(l4arg_o_DEPENDS)

clean:
	$(RM) $(lib_LIBRARIES) $(libl4basic_a_OBJECTS)

install: all install-HEADERS install-LIB
install-LIB:
	-$(MKDIR_P) $(LIBDIR)
	$(INSTALL) $(lib_LIBRARIES) $(LIBDIR)
install-HEADERS:
	-$(MKDIR_P) $(INCLUDEDIR)
	$(INSTALL) $(libl4basic_a_HEADERS) $(INCLUDEDIR)

deinstall: uninstall
remove: uninstall
uninstall:
	for i in $(lib_LIBRARIES); do $(RM) $(LIBDIR)/$$i; done
	for i in $(libl4basic_a_HEADERS); do $(RM) $(INCLUDEDIR)/$$i; done
