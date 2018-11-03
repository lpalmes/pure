OCAMLDEP = ocamldep
LLPP = llpp
OCAMLC = ocamlc.opt
OCAMLOPT = ocamlopt.opt
STDLIB = $(shell $(OCAMLC) -where)

GIT_DESCR := $(or $(shell $(GIT) describe --tags 2>/dev/null),unknown)
VERSION := $(shell echo $(GIT_DESCR) | sed -n 's/v\([0-9]*\).*/\1/p')

# BEST = native
# BEST = byte

LDLIBS += -framework Cocoa 
OCAMLCFLAGS += -pp "refmt --print binary" -I lib

all: clean test

CFLAGS = \
	-Wextra -Wall  \
	-O -g -std=c99 -pedantic-errors \
	-Wsign-compare -Wshadow

lib/cocoa_stubs.o: CFLAGS += -I $(STDLIB)

test: lib/cocoa_stubs.o lib/cocoa.cmx test.cmx
	$(OCAMLOPT) -cclib "$(LDLIBS)" -o $@ $^

clean:
	rm -f lib/*.cm* lib/*.o *.cm* *.o
	rm -f test

.SUFFIXES: .re .rei .cmo .cmi .cmx

.rei.cmi:
	$(OCAMLC) $(OCAMLCFLAGS) -c $<

.re.cmo:
	$(OCAMLC) $(OCAMLCFLAGS) -c -impl $<

.re.cmx:
	$(OCAMLOPT) $(OCAMLCFLAGS) $(OCAMLOPTFLAGS) -c -impl $<

depend:
	$(OCAMLDEP) -I lib lib/*.rei lib/*.re *.rei *.re > .depend

.PHONY: clean depend

include .depend
