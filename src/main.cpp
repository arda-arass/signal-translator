#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <getopt.h>
#include "dsp.hpp"
#include <matplot/matplot.h>

static void usage(){
    std::cout << "\n----------------------------------------\n";
    std::cout << "./signal-translator\n";
    std::cout << "-h (help)\n";
    std::cout << "--test (reads pre-ready file)\n";
    std::cout << "     Example\n";
    std::cout << "       --test fftdata.txt\n";
    std::cout << "-f (frequencies)\n";
    std::cout << "     Example (Hz)\n";
    std::cout << "        50 100 200\n";
    std::cout << "-n (add noise)(default: 10 SNR)\n";
    std::cout << "--snr (specify signal to noise ratio)\n";
    std::cout << "--real (shows real-time spectrum)\n";
    std::cout << "--freq (shows frequency spectrum)\n";
    std::cout << "--psd (shows power spectrum density)\n";
    std::cout << "--no_mirror (no mirror attribute for FFT)\n";
    std::cout << "-----------------------------------------\n\n";
}

static struct option options[] = {
    {"test", required_argument, 0, 1},
    {"real", no_argument, 0, 2},
    {"freq", no_argument, 0, 3},
    {"snr", required_argument, 0, 4},
    {"psd", no_argument, 0, 5},
    {"no_mirror", no_argument, 0 ,6},
    {0, 0, 0, 0 }
};

static int parse_opt(int argc, char **argv, DSP &f) {
    int result;

    opterr = 0;
    while((result = getopt_long(argc, argv, "hfn", options, NULL)) != -1) {
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
            f.snr_f = true;
            f.snr = strtod(optarg, NULL);
            break;
        case 5:
            f.power_f = true;
            break;
        case 6:
            f.no_mirror = true;
            break;
        case 'f':
            f.freq_val_f = true;
            break;
        case 'h':
            usage();
            return 1;
        case 'n':
            f.noise_f = true;
            break;
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

    if (optind != argc && f.freq_val_f) {
        f.sn = argc - optind;

        for (int i = optind; i < argc; ++i)
            f.freq_arr.push_back(strtod(argv[i], NULL));
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
    int m = 0, n = 1;
    matplot::figure(1);

    if(d.real_f)
        m += 1;
    if(d.freq_f)
        m += 1;
    if(d.power_f)
        m += 1;

    if(d.real_f) {
        matplot::subplot(m,1,n);
        matplot::title("Real-Time Spectrum");
        matplot::plot(d.real_x,d.real_y);
        n += 1;
    }
    if(d.freq_f) {
        matplot::subplot(m,1,n);
        matplot::title("Frequency Spectrum");
        matplot::plot(d.fft_x,d.fft_y);
        n += 1;
    }
    if(d.power_f) {
        matplot::subplot(m,1,n);
        matplot::title("Power Spectral Density");
        matplot::plot(d.psd_x,d.psd_y);
    }
    matplot::show();
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

    if(dsp.test_f)
        read_file(dsp);
    else
        dsp.createSignal();

    if(dsp.freq_f)
        dsp.fftw();

    if(dsp.power_f)
        dsp.psd();

    plot(dsp);

    return 0;
}
