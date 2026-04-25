#include "my_project/dsp.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using Clock = std::chrono::steady_clock;
using Signal = my_project::dsp::Signal;

struct Result {
    std::string name;
    double ms;
};

double bench(const std::function<double()>& fn, int iters) {
    volatile double s = 0;
    auto t0 = Clock::now();
    for (int i=0;i<iters;i++) s += fn();
    auto t1 = Clock::now();
    return std::chrono::duration<double,std::milli>(t1-t0).count()/iters;
}

int main(int argc,char** argv)
{
    int iters=10;
    std::string report;
    for(int i=1;i<argc;i++){
        if(std::string(argv[i])=="--iterations") iters=std::stoi(argv[++i]);
        if(std::string(argv[i])=="--report") report=argv[++i];
    }

    constexpr double fs=48000;
    Signal x = my_project::dsp::generate_tone(32768,fs,1500);
    auto h = my_project::dsp::design_lowpass_fir(127,3000/fs);

    std::vector<Result> r;

    r.push_back({"FIR SIMD", bench([&]{auto y=my_project::dsp::convolve(x,h); return y[0];},iters)});
    r.push_back({"FIR FFT", bench([&]{auto y=my_project::dsp::convolve_fft_overlap_save(x,h); return y[0];},iters)});

    r.push_back({"Goertzel scalar", bench([&]{return my_project::dsp::goertzel_power(x,fs,1500);},iters)});
    r.push_back({"Goertzel SIMD", bench([&]{return my_project::dsp::goertzel_power_batch(x,fs,{1500,2000,2500,3000})[0];},iters)});

    if(!report.empty()){
        std::ofstream out(report);
        out<<"# DSP Benchmark Dashboard\n\n";
        out<<"| Method | Time (ms) | Speedup |\n|---|---:|---:|\n";
        double base=r[0].ms;
        for(auto& e:r){
            out<<"| "<<e.name<<" | "<<std::fixed<<std::setprecision(3)<<e.ms
               <<" | "<<base/e.ms<<"x |\n";
        }
    }

    for(auto& e:r)
        std::cout<<e.name<<": "<<e.ms<<" ms\n";
}
