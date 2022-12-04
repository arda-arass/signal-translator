#!/bin/bash

CWD=$(pwd)
FFTW3D=fftw3
FFTW3USR=$CWD/$FFTW3D/usr
FFTW3SOFT=$CWD/$FFTW3D/soft
MATPLOTLIB=https://github.com/lava/matplotlib-cpp.git

main() {
    print "\n---- Configuration has been started ----\n"
    sleep 1

    print "\n---- matplot library is downloading ----\n"
    sleep 1

    git clone $MATPLOTLIB

    fftw3configure
}

fftw3configure() {
    print "\n---- FFTW3 configuration has been started ----\n"
    sleep 1

    mkdir -p $FFTW3USR
    mkdir -p $FFTW3SOFT

    cd $FFTW3SOFT

    wget https://fftw.org/pub/fftw/fftw-3.3.10.tar.gz
    tar -zxvf fftw-3.3.10.tar.gz
    cd fftw-3.3.10
    ./configure --prefix=$FFTW3USR --enable-shared=yes
    make --jobs=8
    make install

    print "\n---- DONE ----\n"
}

print() {
    printf "\e[0;32m%b\e[0m" "$1"
}

case "$1" in
    start)
        main
        ;;    
    "")
        main
        ;;
    *)
        echo "Unknown command [start]"
        exit 1
        ;;
esac