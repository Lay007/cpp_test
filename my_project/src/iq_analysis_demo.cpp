#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Simple CI16 reader + Goertzel demo

std::vector<float> read_ci16(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot open file");

    std::vector<char> raw((std::istreambuf_iterator<char>(f)), {});
    const std::size_t count = raw.size() / sizeof(std::int16_t);

    std::vector<std::int16_t> samples(count);
    std::memcpy(samples.data(), raw.data(), count * sizeof(std::int16_t));

    std::vector<float> i_channel;
    i_channel.reserve(count / 2);

    for (std::size_t k = 0; k + 1 < count; k += 2) {
        const float i = static_cast<float>(samples[k]) / 32768.0f;
        i_channel.push_back(i);
    }

    return i_channel;
}

float goertzel(const std::vector<float>& x, float fs, float freq) {
    const int n = static_cast<int>(x.size());
    const int k = static_cast<int>(0.5f + static_cast<float>(n) * freq / fs);
    const float w = 2.0f * static_cast<float>(M_PI) * static_cast<float>(k) / static_cast<float>(n);
    const float c = 2.0f * std::cos(w);

    float s0 = 0.0f, s1 = 0.0f, s2 = 0.0f;
    for (auto v : x) {
        s0 = v + c * s1 - s2;
        s2 = s1;
        s1 = s0;
    }

    return s1 * s1 + s2 * s2 - c * s1 * s2;
}

float rms(const std::vector<float>& x) {
    if (x.empty()) return 0.0f;
    float acc = 0.0f;
    for (auto v : x) acc += v * v;
    return std::sqrt(acc / static_cast<float>(x.size()));
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: iq_analysis_demo <file> <fs> <freq>\n";
        return 1;
    }

    const std::string file = argv[1];
    const float fs = std::stof(argv[2]);
    const float freq = std::stof(argv[3]);

    const auto signal = read_ci16(file);

    std::cout << "Samples: " << signal.size() << "\n";
    std::cout << "RMS: " << rms(signal) << "\n";

    const float power = goertzel(signal, fs, freq);
    std::cout << "Goertzel power @ " << freq << " Hz: " << power << "\n";

    return 0;
}
