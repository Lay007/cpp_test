#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "dsp_core/dsp.hpp"

namespace {

dsp_core::dsp::Signal read_ci16_i_channel(const std::string& path) {
    std::ifstream input(path, std::ios::binary | std::ios::ate);
    if (!input) {
        throw std::runtime_error("Cannot open IQ file: " + path);
    }

    const auto file_size = input.tellg();
    if (file_size <= 0 || (file_size % static_cast<std::streamoff>(sizeof(std::int16_t))) != 0) {
        throw std::runtime_error("IQ file size must be a positive multiple of int16 sample size.");
    }

    input.seekg(0, std::ios::beg);
    std::vector<std::int16_t> interleaved_iq(static_cast<std::size_t>(file_size) / sizeof(std::int16_t));
    input.read(reinterpret_cast<char*>(interleaved_iq.data()), file_size);
    if (!input) {
        throw std::runtime_error("Failed to read IQ samples from: " + path);
    }

    if (interleaved_iq.size() % 2U != 0U) {
        throw std::runtime_error("IQ file must contain interleaved I/Q int16 samples.");
    }

    dsp_core::dsp::Signal i_channel;
    i_channel.reserve(interleaved_iq.size() / 2U);
    for (std::size_t index = 0; index < interleaved_iq.size(); index += 2U) {
        i_channel.push_back(static_cast<double>(interleaved_iq[index]) / 32768.0);
    }
    return i_channel;
}

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " <file.ci16> <sample_rate_hz> <target_frequency_hz>\n";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    try {
        const std::string file_path = argv[1];
        const double sample_rate_hz = std::stod(argv[2]);
        const double target_frequency_hz = std::stod(argv[3]);

        const auto signal = read_ci16_i_channel(file_path);
        const double signal_rms = dsp_core::dsp::rms(signal);
        const double target_power = dsp_core::dsp::goertzel_power(signal, sample_rate_hz, target_frequency_hz);

        std::cout << "Samples: " << signal.size() << "\n";
        std::cout << "RMS: " << signal_rms << "\n";
        std::cout << "Goertzel power @ " << target_frequency_hz << " Hz: " << target_power << "\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "iq_analysis_demo failed: " << error.what() << "\n";
        return 1;
    }
}
