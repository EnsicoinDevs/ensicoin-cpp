IDIR =../include
BDIR =../bin

CC=g++
CFLAGS=-I$(IDIR) -lcrypto

ODIR=obj

_OBJ = main.o messages.o transaction.o hash.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_DEPS = messages.hpp transaction.hpp hash.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ensicoin-node: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
