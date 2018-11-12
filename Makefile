IDIR=include
BDIR=bin

SUFFIXES += .d
NODEPS:=clean tags svn
SOURCES:=$(shell find src/ -name "*.cpp")
DEPFILES:=$(patsubst %.cpp,%.d,$(SOURCES))

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
	 -include $(DEPFILES)
endif

CXX=clang++
CXXFLAGS=-I$(IDIR) -Wshadow -Wall -Wextra -DASIO_STANDALONE
OFLAGS=-O3 -march=native -flto 
LIBS=-lcryptopp -pthread -lleveldb

ODIR=obj

ensicoin-node: $(OBJ)
	$(CXX) -o $(BDIR)/$@ $^ $(CXXFLAGS) $(LIBS)

include Rulefile

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
