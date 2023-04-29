#include <iostream>
#include <cstring>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <fftw3.h>
#include <string>

#include "dsp.h"

DSP::DSP(int n) : fftSize{n}, tmpIn(n, 0) {}

// Create signal
void DSP::createSignal() {

    const int N = fftSize;         // Number of samples in the signal
    const double fs = 1000;         // Sampling frequency

    std::vector<double> x(N);
    for (int n = 0; n < N; ++n) {
        if(getFlag(flag::FREQ_VAL)){
            for(int i = 0; i < sn; ++i) {
                x[n] += std::sin(2 * M_PI * freqArr.at(i) * n / fs);
            }
        } else { // Default signal
            x[n] = std::sin(2 * M_PI * 50 * n / fs) + 0.5 * std::sin(2 * M_PI * 120 * n / fs);
        }
    }

    if(getFlag(flag::NOISE))
        addGaussianNoise(x);

    dataV = std::move(x);
}

void DSP::fftw() {
    fftw_plan plan;
    plan = fftw_plan_r2r_1d(fftSize, dataV.data(), tmpIn.data(), FFTW_DHT, FFTW_ESTIMATE);
    fftw_execute(plan); // signal to spectrum
    fftw_destroy_plan(plan);

    fftV.clear();
    for(int i = 0; i < (getFlag(flag::NO_MIRROR) ? fftSize/2 : fftSize); ++i){
        fftV.push_back(std::abs(tmpIn[i]/(double)fftSize));
    }
}

// Calculate power spectral density
void DSP::psd() {

    int NoS = fftSize; // Number of samples

    std::vector<double> w = hann_window(NoS);
    std::vector<double> xw(NoS);

    for (int n = 0; n < NoS; n++) {
        xw[n] = dataV.at(n) * w[n];
    }

    // Allocate memory for the FFT output
    fftw_complex* X;
    X = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * NoS);

    // Compute the FFT of the windowed sample
    fftw_plan p = fftw_plan_dft_r2c_1d(NoS, xw.data(), X, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);

    // Calculate the power spectral density
    std::vector<double> psd(NoS / 2 + 1);
    double total_power = 0.0;
    for (int k = 0; k < NoS / 2 + 1; k++) {
        psd[k] = std::pow(my_abs(X[k]), 2);
        total_power += psd[k];
    }

    psdV.clear();
    for (int k = 0; k < NoS / 2 + 1; k++) {
        psdV.push_back(psd[k]/total_power);
    }
}

// Window function (e.g., Hann window)
std::vector<double> DSP::hann_window(int N) {
    std::vector<double> w(N);
    for (int n = 0; n < N; n++) {
        w[n] = 0.5 * (1 - std::cos(2 * M_PI * n / (N - 1)));
    }
    return w;
}

template<typename T>
T DSP::my_abs(T *z) {
    return std::sqrt((*z)*(*z)+(*(z+1))*(*(z+1)));
}

void DSP::addGaussianNoise(std::vector<double> &signal) const {

    // Calculate the power of the signal
    double signalPower = 0.0;
    for (int i = 0; i < signal.size(); i++) {
        signalPower += signal[i] * signal[i];
    }
    signalPower /= signal.size();

    // Calculate the desired noise power based on the SNR
    double noisePower = signalPower / std::pow(10.0, (getFlag(flag::SNR) ? snr : 10.0)/10.0);

    // Generate the noise
    double mean = 0.0;
    double stddev = std::sqrt(noisePower);
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean, stddev);

    // Add the noise to the signal
    for (int i = 0; i < signal.size(); i++) {
        signal[i] += distribution(generator);
    }
}
