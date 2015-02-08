INPUT_FILE=index

HTML_OUT=mgr

build_all: build bibliography build

build: 
	latex ${INPUT_FILE}

bibliography:
	bibtex ${INPUT_FILE}

show: build_all
	xdvi ${INPUT_FILE}.dvi

kshow:build_all
	kdvi ${INPUT_FILE}.dvi

html: 
	latex2html -local_icons -dir ${HTML_OUT} -mkdir ${INPUT_FILE}.tex

html-deploy: html
	scp -r ${HTML_OUT} minus.ds14.agh.edu.pl:public_html/.

pdf:
	pdflatex index.tex

pdf-deploy: pdf
	scp index.pdf minus.ds14.agh.edu.pl:public_html/${HTML_OUT}/.

deploy: pdf-deploy html-deploy

clean:
	rm *.aux *.dvi *.log *.toc *.bbl
