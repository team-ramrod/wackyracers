# Makefile for documentation written with Latex.

../../guidelines.pdf: guidelines.tex
	pdflatex -interaction batchmode guidelines.tex
	mv guidelines.pdf ../..
	
clean:
	rm *.log *.aux *.out
