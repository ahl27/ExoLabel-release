IDIR=./ExoLabel
ODIR=./bin

CFLAGS=-I$(IDIR)
LIBS=

_DEPS = ExoLabel.h FileHandlers.h LoserTree.h PrefixTrie.h FallbackDefines.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = standalonemain.o ExoLabel.o FileHandlers.o LoserTree.o PrefixTrie.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ExoLabel: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)