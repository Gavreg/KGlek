#!/bin/sh
set -e

export latexcmd="latexmk  -pdfxe -interaction=nonstopmode -halt-on-error -synctex=1 -8bit --shell-escape"

workdir=$PWD

for f in ./Images/CG_*/*.tex
do
    [ -f "$f" ] || continue
    
    echo "!==================================!"
    echo "Building file  $f"
    echo "!==================================!"
    
    texfile=$(basename "$f")
    texfiledir=$(dirname "$f")
    
    echo "$f"
    echo "$texfile"
    echo "$texfiledir"

    cd "$texfiledir"
    
    pwd
    eval "$latexcmd \"\$texfile\""
    
    echo ""
    
    cd "$workdir"
done


for f in CG_*.tex
do
    [ -f "$f" ] || continue
    echo "!==================================!"
    echo "Building file  $f"
    echo "!==================================!"
    
    eval $latexcmd "\"\$f\""
done
