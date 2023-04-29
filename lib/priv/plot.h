#ifndef PLOT_H
#define PLOT_H

#include <iosfwd>
#include "dsp.h"

class Plot : public DSP {
public:
    explicit Plot(int n);
    void plot();
};

#endif // PLOT_H