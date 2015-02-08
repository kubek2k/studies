TARGET=main

main.pdf: main.tex simple.tex example.tex
	latex main.tex
	pdflatex main.tex 

clean: 
	@rm -rf main.dvi main.aux main.out main.log example.log example.dvi example.aux simple.log simple.aux simple.dvi main.pdf

view: main.pdf
	xpdf main.pdf
