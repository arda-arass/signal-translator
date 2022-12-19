# signal-translator
* This repository can be used to plot existing signals or create new signals and plot them.
* Also, it allows you to take fft transform of the signal.

## Prerequisites
* It was tested on Ubuntu 20.04 and with anaconda python2.7.
* g++ version was 9.4.0.
* If you want to continue in this way, please follow the instructions below.

## Install Anaconda and create env for python2.7
* sudo apt update
* sudo apt install curl
* mkdir temp && cd temp
* curl https://repo.anaconda.com/archive/Anaconda3-2020.02-Linux-x86_64.sh --output anaconda.sh
* **bash anaconda.sh**
* source ~/.bashrc
* conda create -n py2 python=2.7
* conda activate py2 (optional)
* Ref: https://linuxhint.com/install_anaconda_ubuntu/

## Configure && Compile
* chmod +x configure.sh
* ./configure.sh start
* make
* make install

## Export LD_LIBRARY_PATH
* export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\${HOME}/anaconda3/envs/py2/lib:$(pwd)/fftw3/usr/lib

## Test
> cd ./bin  

> ./signal-translator --test data_04_4_signal.txt --real --freq --no_mirror
> ![FFT and real time spectrum](/img/example.png)


> ./signal-translator -f 50 150 250 -n --snr 1 --real --freq --psd --no_mirror
> ![FFT and real time spectrum](/img/example1.png)