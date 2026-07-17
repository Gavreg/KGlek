#!/bin/sh
set -e

export teximage="teximage"
export workdir=/data
export fonts=/usr/share/fonts

export latexcmd="latexmk  -pdfxe -interaction=nonstopmode -halt-on-error -synctex=1 -8bit --shell-escape"

sudo docker build -t "$teximage" .


for d in ./Images/CG_*
do
    [ -d "$d" ] || continue
    echo "====================="
    echo "Building images in $d"
    echo "====================="
    for f in "$d"/*.tex
    do
        [ -f "$f" ] || continue
        echo "================="
        echo "Building file  $f"
        echo "================="
        sudo docker run --rm -v "$PWD":"$workdir" -w "$workdir" -e f="$f" -e d="$d" -e latexcmd="$latexcmd" "$teximage" sh -c 'cd "$d" && pwd && ls && echo "compiling $f" && eval "$latexcmd \"\$(basename \"\$f\")\""'

        echo ""
    done
done


for f in CG_*.tex
do
    [ -f "$f" ] || continue
     sudo docker run --rm -v "$PWD":"$workdir" -w "$workdir" "$teximage" $latexcmd "$f"
done


sudo docker image rm "$teximage"