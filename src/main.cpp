#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <unistd.h>
#include <getopt.h>

#include "../matplotlib-cpp/matplotlibcpp.h"

#include "dsp.hpp"

namespace plt = matplotlibcpp;

static void usage(){
    std::cout << "./signal-translator\n";
    std::cout << "-h (help)\n";
    std::cout << "--test (reads pre-ready file)\n";
    std::cout << "     Example\n";
    std::cout << "       --test fftdata.txt\n";
    std::cout << "--real (shows real-time spectrum)\n";
    std::cout << "--power (shows power spectrum)\n";
    std::cout << "--no-mirror (no mirror attribute for FFT)\n";
}

static struct option options[] = {
    {"test", required_argument, 0, 1},
    {"real", no_argument, 0, 2},
    {"freq", no_argument, 0, 3},
    {"power", no_argument, 0, 4},
    {"no_mirror", no_argument, 0 ,5},
    {0, 0, 0, 0 }
};

static int parse_opt(int argc, char **argv, DSP &f) {
    int result;

    opterr = 0;
    while((result = getopt_long(argc, argv, "h", options, NULL)) != -1) {
        switch (result) 
        {
        case 1:
            f.test_f = true;
            f.test_file.assign(optarg);
            break;
        case 2:
            f.real_f = true;
            break;
        case 3:
            f.freq_f = true;
            break; 
        case 4:
            f.power_f = true;
            break;
        case 5:
            f.no_mirror = true;
            break;
        case 'h':
            usage();
            return 1;
        case '?':
            if(optopt != 0)
                std::cout << "Invalid switch: -" << optopt << std::endl;
            else
                std::cout << "Invalid switch" << argv[optind - 1] << std::endl;
            return 1;
        default:
            break;
        }
    }

    return 0;
}

static void read_file(DSP &d) {
    int i = 0;
    std::ifstream in("../test/" + d.test_file);
    
    while(!in.eof())
    {
        getline(in, d.text);
        d.val = atof(d.text.c_str());
        d.real_x.push_back(i);
        d.real_y.push_back(d.val);
        d.indata[i] = d.val;
        ++i;
    }
}

static void plot(DSP &d) {
    plt::figure(1);
    if(d.real_f) {
        plt::subplot(2,1,1);
        plt::plot(d.real_x,d.real_y);
    }
    if(d.freq_f) {
        plt::subplot(2,1,2);
        plt::plot(d.fft_x,d.fft_y);
    }
    plt::show();
}

int main(int argc, char **argv)
{
    if(argc < 2) {
        usage();
        exit(EXIT_FAILURE);
    }

    DSP dsp(1024);

    if((parse_opt(argc, argv, dsp)) != 0)
        exit(EXIT_FAILURE);
        
    read_file(dsp);

    dsp.fftw(dsp);

    plot(dsp);

    return 0;
}