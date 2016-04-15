all: report.pdf

%.pdf: %.tex
	pdflatex $<
	pdflatex $<

.PHONY: clean
clean:
	rm -f *.pdf *.cpt *.toc *.aux *.log
