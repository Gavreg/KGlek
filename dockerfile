FROM texlive/texlive:TL2025-historic
RUN apt update && apt install -y fonts-liberation python3.14 clang parallel tree
RUN fc-cache -f -v /usr/share/fonts
WORKDIR /data

