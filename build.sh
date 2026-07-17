#!/bin/sh
set -e

export image=texlive/texlive:TL2025-historic
export container=texbuild-texlive
export workdir=/data
export fonts=/usr/share/fonts

export latexcmd="latexmk -pdfxe -interaction=nonstopmode -synctex=1 -8bit --shell-escape"

sudo docker rm -f "$container" > /dev/null 2>&1 || true

sudo docker create --name "$container" \
  -v "$PWD":"$workdir" \
  -v "$fonts":"$fonts" \
  -w "$workdir" \
  "$image" \
  sleep infinity

sudo docker start "$container"

sudo docker exec "$container" sh -c "fc-cache -f -v $fonts && apt update && DEBIAN_FRONTEND=noninteractive apt install -y python3.14"

sudo docker exec -e latexcmd="$latexcmd"  "$container"  sh -c '\
    find ./Images -type d -name "CG_*" -exec sh -c \
        '\'' \
            cd "$1" && eval "$latexcmd" \
        '\'' _ {} \;\
'


sudo docker exec "$container" \
    sh -c "$latexcmd \
        CG_intro.tex \
        CG_history.tex \
        CG_vectors.tex \
        CG_matrices.tex \
    "
sudo docker rm -f "$container"

