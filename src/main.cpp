#include <iostream>
#include "plot.h"

int main(int argc, char **argv)
{
    Plot plt(1024);

    if(argc < 2) {
        plt.usage();
        exit(EXIT_FAILURE);
    }

    if((plt.parse_opt(argc, argv)) != 0)
        exit(EXIT_FAILURE);

    if(plt.getFlag(Plot::flag::TEST))
        plt.read_file();
    else
        plt.createSignal();

    if(plt.getFlag(Plot::flag::FREQ))
        plt.fftw();

    if(plt.getFlag(Plot::flag::POWER))
        plt.psd();

    plt.plot();

    return 0;
}
