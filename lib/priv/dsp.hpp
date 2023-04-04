#include <vector>

class DSP {
public:
    DSP(int n);
    ~DSP();
    
    void fftw();
    void psd();
    void createSignal();

    /* data read */
    int fftSize;
    double val;
    std::string text;

    /* signal creation */
    int sn;
    double snr;
    std::vector<double> freq_arr;

    /* arguments flags */
    bool test_f;
    bool real_f;
    bool freq_f;
    bool snr_f;
    bool noise_f;
    bool freq_val_f;
    bool power_f;
    bool no_mirror;
    std::string test_file;

    /* Variables */
    std::vector<int> fft_x;
    std::vector<double> fft_y;
    std::vector<int> real_x;
    std::vector<double> real_y;

    int NoS;  // Number of Samples
    std::vector<double> psd_x;
    std::vector<double> psd_y;

    double *indata;

private:
    template<typename T> T my_abs(T *z);
    std::vector<double> hann_window(int N);
    void addGaussianNoise(std::vector<double> &signal)const;

    double *spectrum;
};
