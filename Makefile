IDIR=./ExoLabel
ODIR=./bin

CC=gcc
VPATH=src:src/ExoLabel
LIBS=

DEPS = ExoLabel.h FileHandlers.h LoserTree.h PrefixTrie.h FallbackDefines.h
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

objects = main.o ExoLabel.o FileHandlers.o LoserTree.o PrefixTrie.o
#objects = $(patsubst %,$(ODIR)/%,$(_objects))

exolabel: $(objects)
	cc -o exolabel $(objects)

main.o: ExoLabel.h
ExoLabel.o: FallbackDefines.h ExoLabel.h PrefixTrie.h LoserTree.h FileHandlers.h
PrefixTree.o: FallbackDefines.h PrefixTrie.h
FileHandlers.o: FallbackDefines.h FileHandlers.h
LoserTree.o: FallbackDefines.h LoserTree.h FileHandlers.h

# ExoLabel.o: $(DEPS)
# PrefixTree.o: $(DEPS)
# FileHandlers.o: $(DEPS)
# LoserTree.o: $(DEPS)

.PHONY: clean

clean:
	rm exolabel $(objects)