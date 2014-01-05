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
M_CFLAGS=    -Wall -I. $(CFLAGS)
M_LDFLAGS=   $(LDFLAGS)

# Installation
NAME=        l4basic
DESTDIR=
PREFIX=      /usr/local
LIBDIR=      $(DESTDIR)$(PREFIX)/lib
INCLUDEDIR=  $(DESTDIR)$(PREFIX)/include

# Tasks definition
lib_LIBRARIES=         libl4basic.a
libl4basic_a_OBJECTS=  l4array.o l4array2.o l4file.o l4strv.o l4arg.o \
					   l4list.o
libl4basic_a_HEADERS=  $(libl4basic_a_OBJECTS:.o=.h) l4common.h

check_PROGRAMS=        test-array test-array2 test-file test-strv test-arg \
					   test-list
check_OBJECTS=         $(check_PROGRAMS:=.o)

# Build dependencies
l4array_o_DEPENDS=     l4common.h
l4array2_o_DEPENDS=    l4common.h
l4file_o_DEPENDS=      l4common.h l4array.o
l4strv_o_DEPENDS=      l4common.h l4array.o
l4arg_o_DEPENDS=       l4common.h l4array.o
l4list_o_DEPENDS=      l4common.h

test_array_o_DEPENDS=  l4array.o
test_array2_o_DEPENDS= l4array2.o
test_file_o_DEPENDS=   l4file.o l4array.o
test_strv_o_DEPENDS=   l4strv.o l4array.o
test_arg_o_DEPENDS=    l4arg.o l4array.o l4strv.o
test_list_o_DEPENDS=   l4list.o

# Distribution
EXTRA_DIST=            Makefile README README.zh VERSION
RM_R=                  $(RM) -r
CHMOD_R=               chmod -R 400
CHMOD_W=               chmod -R 755
TAR_PACK=              tar -zcf
TAR_UNPACK=            tar -zxf
TARBALL_EXTENSION=     .tar.gz

.POSIX:
.PHONY: all clean install install-HEADERS install-LIB \
		uninstall deinstall remove dist distcheck
.SUFFIXES: .c.o
.c.o:
	$(CC) $(M_CFLAGS) -c $< -o $@

all: $(lib_LIBRARIES)
check: $(check_PROGRAMS)
	for i in $(check_PROGRAMS); do ./$$i || exit 1; done
libl4basic.a: $(libl4basic_a_OBJECTS)
	$(AR) rcs $@ $(libl4basic_a_OBJECTS)
	$(RANLIB) $@
l4array.o: l4array.c l4array.h $(l4array_o_DEPENDS) 
l4array2.o: l4array2.c l4array2.h $(l4array2_o_DEPENDS)
l4file.o: l4file.c l4file.h $(l4file_o_DEPENDS)
l4strv.o: l4strv.c l4strv.h $(l4strv_o_DEPENDS)
l4arg.o: l4arg.c l4arg.h $(l4arg_o_DEPENDS)
l4list.o: l4list.c l4list.h $(l4list_o_DEPENDS)

test-array: test-array.o $(test_array_o_DEPENDS)
	$(CC) $(M_CFLAGS) test-array.o $(test_array_o_DEPENDS) -o $@ $(M_LDFLAGS)
test-array2: test-array2.o $(test_array2_o_DEPENDS)
	$(CC) $(M_CFLAGS) test-array2.o $(test_array2_o_DEPENDS) -o $@ $(M_LDFLAGS)
test-file: test-file.o $(test_file_o_DEPENDS)
	$(CC) $(M_CFLAGS) test-file.o $(test_file_o_DEPENDS) -o $@ $(M_LDFLAGS)
test-strv: test-strv.o $(test_strv_o_DEPENDS)
	$(CC) $(M_CFLAGS) test-strv.o $(test_strv_o_DEPENDS) -o $@ $(M_LDFLAGS)
test-arg: test-arg.o $(test_arg_o_DEPENDS)
	$(CC) $(M_CFLAGS) test-arg.o $(test_arg_o_DEPENDS) -o $@ $(M_LDFLAGS)
test-list: test-list.o $(test_list_o_DEPENDS)
	$(CC) $(M_CFLAGS) test-list.o $(test_list_o_DEPENDS) -o $@ $(M_LDFLAGS)

clean:
	-$(RM) \
		$(lib_LIBRARIES) $(libl4basic_a_OBJECTS) \
		$(check_PROGRAMS) $(check_OBJECTS)

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

dist:
	$(MKDIR_P) $(NAME)-`cat VERSION`
	for i in $(EXTRA_DIST) $(check_OBJECTS:.o=.c) \
		$(libl4basic_a_HEADERS) $(libl4basic_a_OBJECTS:.o=.c); do \
		$(INSTALL) $$i $(NAME)-`cat VERSION`; done
	$(TAR_PACK) $(NAME)-`cat VERSION`$(TARBALL_EXTENSION) $(NAME)-`cat VERSION`
	$(RM_R) $(NAME)-`cat VERSION`

distcheck: dist
	$(TAR_UNPACK) $(NAME)-`cat VERSION`$(TARBALL_EXTENSION)
	cd $(NAME)-`cat VERSION` && $(MAKE) \
		CC="$(CC)" AR="$(AR)" RANLIB="$(RANLIB)" RM="$(RM)" \
		MKDIR="$(MKDIR)" MKDIR_P="$(MKDIR_P)" INSTALL="$(INSTALL)" \
		CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"
	$(RM_R) $(NAME)-`cat VERSION`
	@echo "--------------------"
	@echo "$(NAME)-`cat VERSION`$(TARBALL_EXTENSION) is ready for distribution!"
	@echo "--------------------"
