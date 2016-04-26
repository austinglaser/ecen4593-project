all: report.pdf

%.pdf: %.tex $(wildcard plots/*) \
             $(wildcard ../../inc/*) \
             $(wildcard ../../src/*)
	pdflatex $<
	pdflatex $<

.PHONY: clean
clean:
	rm -f *.pdf *.cpt *.toc *.aux *.log
