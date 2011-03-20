# Makefile for documentation written with Latex.

docs = guidelines
pdfs = $(addsuffix .pdf, $(docs))
cleaners = $(addprefix clean-, $(docs))

cp_directive = -e "onchange $@ 'cp $@ ..'"

all: $(pdfs)

%.pdf: %.tex .FORCE
	rubber --pdf $(cp_directive) $*

.PHONY: clean
clean: $(cleaners)

clean-%:
	rubber --clean --pdf $*

.FORCE:
