#!/bin/bash
set -e
set -o pipefail

export NPROC=$(nproc || echo 4)  
#export NPROC=1



for arg in $@; do 
    [ $arg = "cpp" ] && buildcpp=true
done



export workdir="$PWD"

compile_and_run_cpp() {
    set -o pipefail
    set -e

    f="$1"

    cd "$cppfiledir"
    
    sudo docker run --rm -v "$workdir":/data teximage bash -c " \
        set -o pipefail; \
        set -e; \
        cppfile=\$(basename "$f"); \
        cppfiledir=\$(dirname "$f"); \
        cppfilename=\$(basename "\$cppfile" .cpp); \
        cd \"\$cppfiledir\"; \
        clang++ -std=c++20 -o \"\$cppfilename\" \"\$cppfile\"; \
        ./\"\$cppfilename\" > /dev/null \
    "
}
export -f  compile_and_run_cpp

compile_tex() {

    set -o pipefail
    set -e

    f="$@"

    texfile=$(basename "$f")
    texfiledir=$(dirname "$f")
    texfilename=$(basename "$texfile" .tex) 
    
    cd "$texfiledir"
    
    if docker run --rm -v "$workdir":/data teximage \
        latexmk   -pdfxe  -interaction=nonstopmode  -recorder- -halt-on-error  -8bit --shell-escape -synctex=0 -cd "$f" > /dev/null 2>&1 
    then 
        rm "$texfilename".log > /dev/null 2>&1 || true 
        true
    else
        mv "$texfilename".log "$texfilename".log.tmp  || true
        docker run --rm -v "$workdir":/data teximage latexmk -C -cd "$f" > /dev/null 2>&1 
        mv "$texfilename".log.tmp  "$texfilename".log  || true
        exit 255
    fi
}
export -f  compile_tex

[ $buildcpp ] && find . -type f -name "*.cpp" -print0 | \
    parallel -0 -j $NPROC --halt-on-error soon,fail=1 --verbose 'compile_and_run_cpp {}'

find . -type d -name "CG_*" -path "./Images*" -exec find {} -name "*.tex" -print0 \;  | \
    parallel -0 -j $NPROC --halt-on-error soon,fail=1 --verbose 'compile_tex {}'

find  . -name "CG_*.tex" -type f -print0 | \
    parallel -0 -j $NPROC --halt-on-error soon,fail=1 --verbose 'compile_tex {}'






