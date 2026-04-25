#include "dsp_core/dsp.hpp"

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using Clock = std::chrono::steady_clock;
using Signal = dsp_core::dsp::Signal;

struct Result {
    std::string name;
    double ms;
};

struct BenchmarkOptions {
    int iterations{10};
    std::filesystem::path report_path;
};

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " [--iterations N] [--report path]\n";
}

BenchmarkOptions parse_arguments(int argc, char** argv) {
    BenchmarkOptions options;
    for (int index = 1; index < argc; ++index) {
        const std::string_view argument(argv[index]);
        if (argument == "--iterations") {
            if (index + 1 >= argc) {
                throw std::invalid_argument("Missing value after --iterations.");
            }
            options.iterations = std::stoi(argv[++index]);
            if (options.iterations <= 0) {
                throw std::invalid_argument("Iterations must be > 0.");
            }
            continue;
        }
        if (argument == "--report") {
            if (index + 1 >= argc) {
                throw std::invalid_argument("Missing value after --report.");
            }
            options.report_path = argv[++index];
            continue;
        }
        if (argument == "--help" || argument == "-h") {
            print_usage(argv[0]);
            std::exit(0);
        }
        throw std::invalid_argument("Unknown argument: " + std::string(argument));
    }
    return options;
}

double bench(const std::function<double()>& fn, int iters) {
    volatile double s = 0.0;
    auto t0 = Clock::now();
    for (int i = 0; i < iters; ++i) s += fn();
    auto t1 = Clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count() / static_cast<double>(iters);
}

int main(int argc, char** argv) {
    try {
        const auto options = parse_arguments(argc, argv);

        constexpr double fs = 48'000.0;
        Signal x = dsp_core::dsp::generate_tone(32'768, fs, 1'500.0);
        auto h = dsp_core::dsp::design_lowpass_fir(127, 3'000.0 / fs);

        std::vector<Result> results;
        results.push_back({"FIR direct", bench([&] { return dsp_core::dsp::convolve(x, h).front(); }, options.iterations)});
        results.push_back({"FIR overlap-save FFT", bench([&] { return dsp_core::dsp::convolve_fft_overlap_save(x, h).front(); }, options.iterations)});
        results.push_back({"Goertzel scalar", bench([&] { return dsp_core::dsp::goertzel_power(x, fs, 1'500.0); }, options.iterations)});
        results.push_back({"Goertzel batched", bench([&] { return dsp_core::dsp::goertzel_power_batch(x, fs, {1'500.0, 2'000.0, 2'500.0, 3'000.0}).front(); }, options.iterations)});

        if (!options.report_path.empty()) {
            if (const auto parent = options.report_path.parent_path(); !parent.empty()) {
                std::filesystem::create_directories(parent);
            }
            std::ofstream out(options.report_path);
            if (!out) {
                throw std::runtime_error("Cannot open benchmark report for writing.");
            }

            out << "# DSP Benchmark Dashboard\n\n";
            out << "| Method | Time (ms) | Speedup |\n|---|---:|---:|\n";
            const double baseline_ms = results.front().ms;
            for (const auto& entry : results) {
                out << "| " << entry.name << " | " << std::fixed << std::setprecision(3) << entry.ms
                    << " | " << baseline_ms / entry.ms << "x |\n";
            }
        }

        for (const auto& entry : results) {
            std::cout << entry.name << ": " << std::fixed << std::setprecision(3) << entry.ms << " ms\n";
        }
        return 0;
    } catch (const std::exception& error) {
        print_usage(argv[0]);
        std::cerr << "Benchmark failed: " << error.what() << '\n';
        return 1;
    }
}
