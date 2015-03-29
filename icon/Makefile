
MAINFILE := icon



OPTIONS := -halt-on-error -interaction=nonstopmode -shell-escape

all: svg png

pdf:
	pdflatex $(OPTIONS) $(MAINFILE)

# http://tex.stackexchange.com/questions/51757/how-can-i-use-tikz-to-make-standalone-svg-graphics
svg:
	pdflatex $(OPTIONS) '\PassOptionsToClass{convert={outfile=$(MAINFILE).svg}}{standalone} \input{$(MAINFILE)}'

# http://tex.stackexchange.com/questions/11866/compile-a-latex-document-into-a-png-image-thats-as-short-as-possible#11880
png:
	pdflatex $(OPTIONS) '\PassOptionsToClass{convert={outfile=$(MAINFILE).png,size=86x86}}{standalone} \input{$(MAINFILE)}'



clean:
	- find . | egrep ".*((\.(aux|idx|ind|ilg|log|blg|bbl|toc|lof|lot|dvi|tlf|tlt|bcf|run\.xml))|~)$$" | xargs rm -f
	- rm -f $(MAINFILE).out thumb???.png thumbdta.tex thumbpdf.*
