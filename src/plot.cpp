#include <vector>
#include <matplot/matplot.h>
#include "plot.h"

Plot::Plot(int n) : DSP(n) {}

void Plot::plot() {
    int m = 0, n = 1;
    std::vector<int> xAxis(fftSize);
    matplot::figure(1);

    std::iota(xAxis.begin(), xAxis.end(), 0);

    if(getFlag(flag::REAL))
        m += 1;
    if(getFlag(flag::FREQ))
        m += 1;
    if(getFlag(DSP::flag::POWER))
        m += 1;

    if(getFlag(flag::REAL)) {
        matplot::subplot(m,1,n);
        matplot::title("Real-Time Spectrum");
        matplot::plot(xAxis,dataV);
        n += 1;
    }
    if(getFlag(flag::FREQ)) {
        matplot::subplot(m,1,n);
        matplot::title("Frequency Spectrum");
        matplot::plot(xAxis,fftV);
        n += 1;
    }
    if(getFlag(flag::POWER)) {
        matplot::subplot(m,1,n);
        matplot::title("Power Spectral Density");
        matplot::plot(xAxis,psdV);
    }
    matplot::show();
}
