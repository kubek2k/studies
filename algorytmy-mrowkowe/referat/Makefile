NAME=mrowki

all: mrowki.tex
	latex $(NAME).tex
	dvipdf $(NAME).dvi $(NAME).pdf

clean: 
	@rm *.dvi *.log *.aux

view: all
	xdvi $(NAME).dvi
