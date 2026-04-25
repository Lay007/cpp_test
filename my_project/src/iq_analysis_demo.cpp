#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>

// Simple CI16 reader + Goertzel demo

std::vector<float> read_ci16(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot open file");

    std::vector<int16_t> raw((std::istreambuf_iterator<char>(f)), {});
    size_t count = raw.size() / sizeof(int16_t);

    std::vector<int16_t> samples(count);
    std::memcpy(samples.data(), raw.data(), raw.size());

    std::vector<float> i_channel;
    i_channel.reserve(count / 2);

    for (size_t k = 0; k + 1 < count; k += 2) {
        float i = samples[k] / 32768.0f;
        i_channel.push_back(i);
    }

    return i_channel;
}

float goertzel(const std::vector<float>& x, float fs, float freq) {
    int n = x.size();
    int k = static_cast<int>(0.5 + n * freq / fs);
    float w = 2 * M_PI * k / n;
    float c = 2 * std::cos(w);

    float s0 = 0, s1 = 0, s2 = 0;
    for (auto v : x) {
        s0 = v + c * s1 - s2;
        s2 = s1;
        s1 = s0;
    }

    return s1*s1 + s2*s2 - c*s1*s2;
}

float rms(const std::vector<float>& x) {
    float acc = 0;
    for (auto v : x) acc += v*v;
    return std::sqrt(acc / x.size());
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: iq_analysis_demo <file> <fs> <freq>\n";
        return 1;
    }

    std::string file = argv[1];
    float fs = std::stof(argv[2]);
    float freq = std::stof(argv[3]);

    auto signal = read_ci16(file);

    std::cout << "Samples: " << signal.size() << "\n";
    std::cout << "RMS: " << rms(signal) << "\n";

    float power = goertzel(signal, fs, freq);
    std::cout << "Goertzel power @ " << freq << " Hz: " << power << "\n";

    return 0;
}
