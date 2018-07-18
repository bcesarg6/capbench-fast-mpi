FILE_NAME = main

all:
	pdflatex $(FILE_NAME).tex
	bibtex $(FILE_NAME).aux
	pdflatex $(FILE_NAME).tex

clean:
	rm $(FILE_NAME).log $(FILE_NAME).aux  $(FILE_NAME).pdf $(FILE_NAME).bbl $(FILE_NAME).blg
