# Makefile for documentation written with Latex.

# You should be able to simply add your main document file here, rubber takes
# care of detecting all dependencies automatically.
docs = guidelines

pss      = $(addsuffix    .ps, $(docs))
pdfs     = $(addsuffix   .pdf, $(docs))
dvis     = $(addsuffix   .dvi, $(docs))
cleaners = $(addprefix clean-, $(docs))

default: all-pdf

all-ps:  $(pss)
all-pdf: $(pdfs)
all-dvi: $(dvis)

.PHONY: default all-pdf all-ps all-dvi

%.pdf: %.tex .FORCE
	rubber --pdf -e "onchange $@ 'cp $@ ..'" $*

%.dvi: %.tex .FORCE
	rubber -e "onchange $@ 'cp $@ ..'" $*

%.ps: %.tex .FORCE
	rubber --ps -e "onchange $@ 'cp $@ ..'" $*

.PHONY: clean
clean: $(cleaners)

clean-%:
	rubber --clean $*

.FORCE:
