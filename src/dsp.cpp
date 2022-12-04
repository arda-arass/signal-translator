#include "dsp.hpp"

DSP::DSP(int n) : test_f{false}, real_f{false}, freq_f{false}, power_f{false}, no_mirror{false} {
    this->fftSize = n;
    this->indata = new double[n];
    memset(this->indata, 0, n*sizeof(double));
    this->spectrum = new double[n];
    memset(this->spectrum, 0, n*sizeof(double));
}

DSP::~DSP(){
    delete[] this->indata;
    delete[] this->spectrum;
}

void DSP::fftw(DSP &dsp) {
    fftw_plan plan;
    plan = fftw_plan_r2r_1d(dsp.fftSize, dsp.indata, this->spectrum, FFTW_DHT, FFTW_ESTIMATE);
    fftw_execute(plan); // signal to spectrum
    fftw_destroy_plan(plan);

    dsp.fft_x.clear();
    dsp.fft_y.clear();
    for(int i = 0; i < (dsp.no_mirror ? dsp.fftSize/2 : dsp.fftSize); ++i){
        dsp.fft_x.push_back(i);
        dsp.fft_y.push_back(std::abs(spectrum[i]/(double)dsp.fftSize));
    }
}