all: report.pdf

%.pdf: %.tex $(wildcard plots/*)
	pdflatex $<
	pdflatex $<

.PHONY: clean
clean:
	rm -f *.pdf *.cpt *.toc *.aux *.log
