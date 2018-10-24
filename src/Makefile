IDIR =../include
BDIR =../bin

CC=g++
CFLAGS=-I$(IDIR) -lcryptopp -pthread -DASIO_STANDALONE

ODIR=obj

_OBJ = main.o messages.o transaction.o crypto.o script.o blocks.o node.o hashmemory.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_DEPS = messages.hpp transaction.hpp crypto.hpp script.hpp blocks.hpp node.hpp hashmemory.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ensicoin-node: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
