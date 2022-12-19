#include "dsp.hpp"

DSP::DSP(int n) : NoS{n}, test_f{false}, real_f{false}, freq_f{false}, snr_f{false}, power_f{false}, no_mirror{false} {
    this->fftSize = n;
    this->indata = new double[n];
    memset(this->indata, 0, n*sizeof(double));
    this->spectrum = new double[n];
    memset(this->spectrum, 0, n*sizeof(double));
}

DSP::~DSP() {
    delete[] this->indata;
    delete[] this->spectrum;
}

/********************************************************/
/******************* PUBLIC FUNCTIONS *******************/
/********************************************************/

// Create signal
void DSP::createSignal(DSP &dsp) {

    const int N = 1024;         // Number of samples in the signal
    const double fs = 1000;     // Sampling frequency

    std::vector<double> x(N);
    for (int n = 0; n < N; ++n) {

        if(dsp.freq_val_f){
            for(int i = 0; i < sn; ++i) {
                x[n] += std::sin(2 * M_PI * freq_arr.at(i) * n / fs);
            }
        } else { // Default signal
            x[n] = std::sin(2 * M_PI * 50 * n / fs) + 0.5 * std::sin(2 * M_PI * 120 * n / fs);
        }

        dsp.real_x.push_back(n);
    }

    if(dsp.noise_f)
        this->addGaussianNoise(dsp, x);

    dsp.real_y.assign(x.begin(), x.end());
    std::copy(dsp.real_y.begin(), dsp.real_y.end(), dsp.indata);
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

// Calculate power spectral density
void DSP::psd(DSP &dsp) {

    std::vector<double> w = hann_window(NoS);
    std::vector<double> xw(NoS);

    for (int n = 0; n < NoS; n++) {
        xw[n] = dsp.real_y.at(n) * w[n];
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

    dsp.psd_x.clear();
    dsp.psd_y.clear();
    for (int k = 0; k < NoS / 2 + 1; k++) {
        dsp.psd_y.push_back(psd[k]/total_power);
        dsp.psd_x.push_back(k);
    }
}

/********************************************************/
/****************** PRIVATE FUNCTIONS *******************/
/********************************************************/

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

void DSP::addGaussianNoise(const DSP &dsp, std::vector<double> &signal) {

    // Calculate the power of the signal
    double signalPower = 0.0;
    for (int i = 0; i < signal.size(); i++) {
        signalPower += signal[i] * signal[i];
    }
    signalPower /= signal.size();

    // Calculate the desired noise power based on the SNR
    double noisePower = signalPower / std::pow(10.0, (dsp.snr_f ? dsp.snr : 10.0)/10.0);

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
