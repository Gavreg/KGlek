#!/bin/bash
set -e
set -o pipefail

export NPROC=$(nproc || echo 4)  
#export NPROC=1

export workdir="$PWD"

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

rm -rf errors || true
mkdir  errors || true

find ./Images/CG_*/*.tex -type f -print0 | \
    parallel -0 -j $NPROC --halt-on-error soon,fail=1 --verbose 'compile_tex {}'

find  . -name "CG_*.tex" -type f -print0 | \
    parallel -0 -j $NPROC --halt-on-error soon,fail=1 --verbose 'compile_tex {}'





# #!/bin/bash
# set -e
# set -o pipefail

# export NPROC=$(nproc || echo 4)  
# #export NPROC=4


# compile_tex() {

#     set -o pipefail
#     set -e

#     f="$@"

#     workdir=$PWD

#     texfile=$(basename "$f")
#     texfiledir=$(dirname "$f")
#     texfilename=$(basename "$texfile" .tex) 
    
#     cd "$texfiledir"

#     if latexmk  -pdfxe  -interaction=nonstopmode  -recorder- -halt-on-error  -8bit --shell-escape -synctex=0  "$texfile" > /dev/null 2>&1; then #| tee "$workdir"/errors/"$texfile".log  
#         rm -f "$texfilename".log || true
#     else
#         mv "$texfilename".log "$texfilename".log.tmp
#         latexmk -C "$texfile"
#         mv "$texfilename".log.tmp "$texfilename".log
#         exit 255
#     fi

# }

# export -f  compile_tex

# rm -rf errors || true
# mkdir  errors || true

# find ./Images/CG_*/*.tex -type f -print0 | \
#     parallel -0 -j $NPROC --halt-on-error soon,fail=1 --verbose 'compile_tex {}'

# find  . -name "CG_*.tex" -type f -print0 | \
#     parallel -0 -j $NPROC --halt-on-error soon,fail=1 --verbose 'compile_tex {}'




