IDIR =../include
BDIR =../bin

CC=clang++
CFLAGS=-I$(IDIR) -Wshadow -Wall -Wextra -DASIO_STANDALONE
OFLAGS=-O3 -march=native -flto 
LIBS=-lcryptopp -pthread

ODIR=obj
VPATH = transaction message .

_OBJ = main.o messagehandler.o blockchain.o mempool.o transactionid.o transactioninput.o transactionoutput.o getmempool.o getblocks.o transactionmessage.o blockmessage.o connection.o notfound.o inv.o whoami.o messages.o getdata.o transaction.o crypto.o script.o blocks.o node.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_DEPS = messages.hpp blockchain.hpp constants.hpp mempool.hpp connection.hpp transaction.hpp crypto.hpp script.hpp blocks.hpp node.hpp hashmemory.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: %.cpp $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

ensicoin-node: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
