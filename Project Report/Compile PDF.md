# Compile pdf from tex and bib file

pdflatex ProjectReport.tex
bibtex ProjectReport.aux
makeglossaries ProjectReport.aux
pdflatex ProjectReport.tex
pdflatex ProjectReport.tex