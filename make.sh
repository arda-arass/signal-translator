#!/bin/sh

BUILD_DIRECTORY=build

configure() {

    if [ -d "$BUILD_DIRECTORY" ]; then
        print "$BUILD_DIRECTORY directory already exists. Overriding..."
    else
        print "$BUILD_DIRECTORY directory will be created!"
        mkdir $BUILD_DIRECTORY
    fi

    if [ "$1" = "release" ]
    then
        print "RELEASE MODE"
        cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    elif [ "$1" = "debug" ]
    then
        print "DEBUG MODE"
        cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
    else
        cmake -S . -B build
    fi
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
    printf "\e[0;32m%b\e[0m" "$1" "\n"
}

case "$1" in
    configure)
        configure "$2"
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
