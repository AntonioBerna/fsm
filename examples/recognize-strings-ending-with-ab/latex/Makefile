COMPILER=lualatex
OUTPUT_PDF=recognize-strings-ending-with-ab
INPUT_FILE=recognize-strings-ending-with-ab.tex

all: pdf png

pdf:
	$(COMPILER) -jobname=$(OUTPUT_PDF) $(INPUT_FILE)
	$(COMPILER) -jobname=$(OUTPUT_PDF) $(INPUT_FILE)

png: pdf
	pdftoppm $(OUTPUT_PDF).pdf $(OUTPUT_PDF) -png

clean:
	$(RM) *.aux *.log *.out *.fls *.fdb_latexmk *.toc *.gz

clean_pdf:
	$(RM) $(OUTPUT_PDF).pdf

clean_png:
	$(RM) $(OUTPUT_PDF)-*.png

clean_all: clean clean_pdf clean_png

.PHONY: all pdf png clean clean_pdf clean_png clean_all
