#ifndef DSP_H
#define DSP_H

#include <iosfwd>
#include <parse.h>

class DSP : public Parse {
public:
    explicit DSP(int n);

    void fftw();
    void psd();
    void createSignal();

private:
    std::vector<double> tmpIn;

    template<typename T> T my_abs(T *z);
    std::vector<double> hann_window(int N);
    void addGaussianNoise(std::vector<double> &signal)const;

protected:
    int fftSize;
    std::vector<double> fftV;
    std::vector<double> psdV;
};

#endif // DSP_H
