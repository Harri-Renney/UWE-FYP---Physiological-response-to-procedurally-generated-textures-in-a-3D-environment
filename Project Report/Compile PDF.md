# Compile pdf from tex and bib file

pdflatex ProjectReport.tex

bibtex ProjectReport.aux

makeglossaries ProjectReport

pdflatex ProjectReport.tex

pdflatex ProjectReport.tex