CC=cc
AR=ar
RM=rm
CFLAGS=-Wall -g -I.
OBJ=
LIBFILE=libl4arg.a

.PHONY: all clean

all: $(LIBFILE)
$(LIBFILE): $(OBJ)
	$(AR) rcs $(LIBFILE) $(OBJ)
clean:
	$(RM) -f $(LIBFILE) $(OBJ)
