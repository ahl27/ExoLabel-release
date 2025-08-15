IDIR=src/ExoLabel
ODIR=bin

CC=gcc
VPATH=src:src/ExoLabel:bin

_DEPS = ExoLabel.h FileHandlers.h LoserTree.h PrefixTrie.h FallbackDefines.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_objects = main.o ExoLabel.o FileHandlers.o LoserTree.o PrefixTrie.o
objects = $(patsubst %,$(ODIR)/%,$(_objects))

exolabel: $(objects)
	cc -lz -o exolabel $(objects)
$(ODIR):
	mkdir -p $(ODIR)
$(ODIR)/%.o: %.c $(_DEPS)
	@mkdir -p $(ODIR)
	@$(CC) $(CFLAGS) -c -o $@ $<
# $(ODIR)/main.o: ExoLabel.h
# $(ODIR)/ExoLabel.o: FallbackDefines.h ExoLabel.h PrefixTrie.h LoserTree.h FileHandlers.h
# $(ODIR)/PrefixTrie.o: FallbackDefines.h PrefixTrie.h
# $(ODIR)/FileHandlers.o: FallbackDefines.h FileHandlers.h
# $(ODIR)/LoserTree.o: FallbackDefines.h LoserTree.h FileHandlers.h

# ExoLabel.o: $(DEPS)
# PrefixTree.o: $(DEPS)
# FileHandlers.o: $(DEPS)
# LoserTree.o: $(DEPS)

.PHONY: clean

clean:
	rm -rf $(OBJDIR) && rm -f exolabel