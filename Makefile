IDIR=include
BDIR=bin

SUFFIXES += .d
NODEPS:=clean tags svn
SOURCES:=$(shell find src/ -name "*.cpp")
DEPFILES:=$(patsubst %.cpp,%.d,$(SOURCES))

CXX=clang++
CXXFLAGS=-I$(IDIR) -Wshadow -Wall -Wextra -DASIO_STANDALONE
OFLAGS=-O3 -march=native -flto 
LIBS=-lcryptopp -pthread -lleveldb

ODIR=obj

OBJS:=$(patsubst %.cpp, obj/%.o, $(shell find src/ -type f -printf "%f "))

ensicoin-node: $(OBJS)
	$(CXX) -o $(BDIR)/$@ $^ $(CXXFLAGS) $(LIBS)

include Rulefile

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
