#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <getopt.h>

#include "parse.h"

static struct option options[] = {
    {"test", required_argument, 0, 1},
    {"real", no_argument, 0, 2},
    {"freq", no_argument, 0, 3},
    {"snr", required_argument, 0, 4},
    {"psd", no_argument, 0, 5},
    {"no_mirror", no_argument, 0 ,6},
    {0, 0, 0, 0 }
};

Parse::Parse() : flg(static_cast<int>(flag::NO_MIRROR) + 1, false), 
    dataV{}, freqArr{}, testFile{}, sn(0), snr(1.0) 
{
}

void Parse::usage()const {
    std::cout << "\n----------------------------------------\n"
              << "./signal-translator\n"
              << "-h (help)\n"
              << "--test (reads pre-ready file)\n"
              << "     Example\n"
              << "       --test fftdata.txt\n"
              << "-f (frequencies)\n"
              << "     Example (Hz)\n"
              << "        50 100 200\n"
              << "-n (add noise)(default: 10 SNR)\n"
              << "--snr (specify signal to noise ratio)\n"
              << "--real (shows real-time spectrum)\n"
              << "--freq (shows frequency spectrum)\n"
              << "--psd (shows power spectrum density)\n"
              << "--no_mirror (no mirror attribute for FFT)\n"
              << "-----------------------------------------\n\n";
}

int Parse::parse_opt(int argc, char **argv) {
    int result;
    std::vector<double> freq_arr{};

    opterr = 0;
    while((result = getopt_long(argc, argv, "hfn", options, NULL)) != -1) {
        switch (result) 
        {
        case 1:
            setFlag(flag::TEST, true);
            setTestFile(std::string{optarg});
            break;
        case 2:
            setFlag(flag::REAL, true);
            break;
        case 3:
            setFlag(flag::FREQ, true);
            break; 
        case 4:
            setFlag(flag::SNR, true);
            setSNR(strtod(optarg, NULL));
            break;
        case 5:
            setFlag(flag::POWER, true);
            break;
        case 6:
            setFlag(flag::NO_MIRROR, true);
            break;
        case 'f':
            setFlag(flag::FREQ_VAL, true);
            break;
        case 'h':
            usage();
            return 1;
        case 'n':
            setFlag(flag::NOISE, true);
            break;
        case '?':
            if(optopt != 0)
                std::cout << "Invalid switch: -" << optopt << "\n";
            else
                std::cout << "Invalid switch" << argv[optind - 1] << "\n";
            return 1;
        default:
            break;
        }
    }

    if (optind != argc && getFlag(flag::FREQ_VAL)) {
        // f.sn = argc - optind;
        setSN(argc - optind);

        for (int i = optind; i < argc; ++i)
            freq_arr.push_back(strtod(argv[i], NULL));
        setFreqArr(std::move(freq_arr));
    }

    return 0;
}

void Parse::read_file() {
    std::string text;
    std::ifstream in("../test/" + getTestFile());
    
    while(!in.eof())
    {
        getline(in, text);
        dataV.push_back(atof(text.c_str()));
    }
}
