#ifndef PARSE_H
#define PARSE_H

#include <iosfwd>
#include <vector>

class Parse {
public:
    enum class flag {
        TEST,
        REAL,
        FREQ,
        SNR,
        NOISE,
        FREQ_VAL,
        POWER,
        NO_MIRROR
    };

    Parse();

    void usage()const;
    void read_file();
    int parse_opt(int argc, char **argv);
    bool getFlag(flag f)const;

private:
    void setFlag(flag f, bool b);
    void setSN(int n);
    void setSNR(double n);
    void setFreqArr(std::vector<double> &&arr);
    void setTestFile(std::string &&file);
    std::string getTestFile()const;

protected:
    std::vector<bool> flg;
    std::vector<double> dataV;
    std::vector<double> freqArr;
    std::string testFile;
    int sn;     // Signal Number
    double snr; // Signal to Noise Ratio
};

inline void Parse::setFlag(flag f, bool b) 
{
    flg[static_cast<int>(f)] = b;
}

inline bool Parse::getFlag(flag f) const 
{
    return flg[static_cast<int>(f)];
}

inline void Parse::setSN(int n) 
{
    sn = n;
}

inline void Parse::setSNR(double n) 
{
    snr = n;
}

inline void Parse::setFreqArr(std::vector<double> &&arr) 
{
    freqArr = std::move(arr);
}

inline void Parse::setTestFile(std::string &&file) 
{
    testFile = std::move(file);
}

inline std::string Parse::getTestFile()const 
{
    return testFile;
}

#endif // PARSE_H
