SOURCES=main.tex abstract.tex intro.tex environments.tex problems.tex biblio.bib
FIGS=fig1.fig

default: all

all: main.dvi

figs: ${FIGS}
	./gengfx

main.dvi: ${SOURCES} figs
	- rm main.bbl main.blg
	latex main.tex
	bibtex main
	latex main.tex
	bibtex main
	latex main.tex
	dvips main.dvi
	dvipdf main.dvi

view: main.dvi
	xdvi main.dvi

clean:
	- rm *.eps *.bbl *.blg *.dvi *.toc *.log *.aux
