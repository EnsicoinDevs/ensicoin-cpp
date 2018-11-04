IDIR =../include
BDIR =../bin

CC=g++
CFLAGS=-I$(IDIR) -Werror -Wall -Wextra -lcryptopp -pthread -DASIO_STANDALONE

define cc-command
$(CC) -c -o $@ $< $(CFLAGS)
endef

ODIR=obj
VPATH = message .

_OBJ = main.o getmempool.o getblocks.o transactionmessage.o blockmessage.o connection.o notfound.o inv.o whoami.o messages.o getdata.o transaction.o crypto.o script.o blocks.o node.o hashmemory.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_DEPS = messages.hpp connection.hpp transaction.hpp crypto.hpp script.hpp blocks.hpp node.hpp hashmemory.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: %.cpp $(DEPS) 
	$(cc-command)

ensicoin-node: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
