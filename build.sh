#!/bin/sh
set -e

export latexcmd="latexmk  -pdfxe -interaction=nonstopmode -halt-on-error -synctex=1 -8bit --shell-escape"

workdir=$(pwd)

for f in ./Images/CG_*/*.tex
do
    [ -f "$f" ] || continue
    
    echo "!==================================!"
    echo "Building file  $f"
    echo "!==================================!"
    
    texfile=$(basename "$f")
    texfiledir=&(dirname "$f")
    
    cd "$texfiledir"
    
    echo "$latexcmd \"\$(basename \"\$f\")\""
    
    echo ""
    
    cd "$workdir"
done


for f in CG_*.tex
do
    [ -f "$f" ] || continue
    echo "!==================================!"
    echo "Building file  $f"
    echo "!==================================!"
    
    echo $latexcmd "$f"
done
