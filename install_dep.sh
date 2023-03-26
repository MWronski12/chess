#!/bin/sh

# cmake=3.22.1
# libsfml-dev
# libudev-dev
# libflac-dev
# libvorbis-dev
# libopenal-dev
# libxcursor-dev
# libfreetype6-dev
sudo apt update
cat requirements.txt | xargs sudo apt-get -y install