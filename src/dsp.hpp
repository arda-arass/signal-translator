#include <iostream>
#include <vector>
#include <cstring>
#include "../fftw3/usr/include/fftw3.h"

class DSP {
public:
    DSP(int n);
    ~DSP();
    
    void fftw(DSP &dsp);
    void createSignal();

    /* data read */
    int fftSize;
    double val;
    std::string text;

    /* arguments flags */
    bool test_f;
    bool real_f;
    bool freq_f;
    bool power_f;
    bool no_mirror;
    std::string test_file;

    /* Variables */
    std::vector<int> fft_x;
    std::vector<double> fft_y;
    std::vector<int> real_x;
    std::vector<double> real_y;

    double *indata;

private:
    double *spectrum;
};