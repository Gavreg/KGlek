FORFILES /P . /M *.tex /C "CMD /C START /B xelatex.exe -synctex=1 -interaction=nonstopmode -shell-escape -8bit @file"
pause