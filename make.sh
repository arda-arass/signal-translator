#!/bin/sh

BUILD_DIRECTORY=build

configure() {
    mkdir $BUILD_DIRECTORY
    cmake -S . -B build
}

build() {

    if [ ! -d "$BUILD_DIRECTORY" ]; then
        echo "$BUILD_DIRECTORY directory does not exist. Configure again"
        exit 1
    fi

    make -C build
}

run() {

    if [ ! -d "$BUILD_DIRECTORY" ]; then
        echo "$BUILD_DIRECTORY directory does not exist. Configure and build again"
        exit 1
    fi

    ./$BUILD_DIRECTORY/signal-translator
}

clean() {
    rm -rf build
    echo "remove build"
}


print() {
    printf "\e[0;32m%b\e[0m" "$1"
}

case "$1" in
    configure)
        configure
        ;;  
    build)
        build
        ;;
    run)
        run
        ;;
    clean)
        clean
        ;;
    *)
        echo "Unknown command [configure|build|run|clean]"
        exit 1
        ;;
esac
