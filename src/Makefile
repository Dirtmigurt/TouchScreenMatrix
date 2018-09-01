IDIR =../include
CC=g++
CFLAGS=-std=c++11 -I$(IDIR)

ODIR=obj
LDIR=../lib
# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

# These commands build the RGB Matrix lib, only run once.
#$(RGB_LIBRARY): FORCE
#	$(MAKE) -C $(RGB_LIBDIR)

_DEPS = InputEvent.h InputEventReader.h BootScreen.h MainMenu.h MenuItem.h DisplayMain.h Point.h VisibleObject.h VisibleObjectManager.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o InputEvent.o InputEventReader.o BootScreen.o MainMenu.o MenuItem.o DisplayMain.o VisibleObject.o VisibleObjectManager.o
OBJ  = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c* $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ run

FORCE:
.PHONY: FORCE