#include "my_project/dsp.hpp"

#include <cmath>
#include <iostream>

int main() {
    constexpr double kFs = 48'000.0;
    constexpr std::size_t kSamples = 2'048;

    auto tone = my_project::dsp::generate_tone(kSamples, kFs, 1'500.0);
    auto taps = my_project::dsp::design_lowpass_fir(127, 3'000.0 / kFs);
    auto filtered = my_project::dsp::convolve(tone, taps);
    auto resampled = my_project::dsp::resample_rational(tone, 3, 2);

    const double in_rms = my_project::dsp::rms(tone);
    const double out_rms = my_project::dsp::rms(filtered);
    const double passband = my_project::dsp::frequency_response_magnitude(taps, 1'500.0 / kFs);
    const double stopband = my_project::dsp::frequency_response_magnitude(taps, 10'000.0 / kFs);

    std::cout << "Input RMS: " << in_rms << '\n';
    std::cout << "Filtered RMS: " << out_rms << '\n';
    std::cout << "Passband |H(f)|: " << passband << '\n';
    std::cout << "Stopband |H(f)|: " << stopband << '\n';
    std::cout << "Resampled size (3/2): " << resampled.size() << '\n';
    std::cout << "Demo complete." << std::endl;
    return (std::isfinite(in_rms) && std::isfinite(out_rms)) ? 0 : 1;
}
