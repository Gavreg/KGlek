#!/bin/bash
set -e

export latexcmd="latexmk -g -pdfxe -interaction=nonstopmode -halt-on-error -synctex=1 -8bit --shell-escape"
export NPROC=$(nproc || echo 4)  

export workdir=$PWD

compile_tex() {
    f="$1"
    texfile=$(basename "$f")
    texfiledir=$(dirname "$f")
    
    echo "!==================================!"
    echo "Building file  $f"
    echo "!==================================!"

    cd "$texfiledir"
    eval "$latexcmd \"$texfile\""
}

export -f  compile_tex

find ./Images/CG_*/*.tex CG_*.tex -type f -print0 2>/dev/null | \
    xargs -P "$NPROC" -0 -I {} bash -c 'compile_tex "$@"' _ {}


find  CG_*.tex -type f -print0 2>/dev/null | \
    xargs -P "$NPROC" -0 -I {} bash -c 'compile_tex "$@"' _ {}
