#include "my_project/dsp.hpp"

#include <random>

#include <gtest/gtest.h>

TEST(DspGoertzel, DetectsTargetToneInsideNoisyMixture) {
    constexpr double kFs = 48'000.0;
    constexpr std::size_t kSamples = 2'048;

    auto signal = my_project::dsp::generate_tone(kSamples, kFs, 1'640.625, 1.0);
    const auto interferer = my_project::dsp::generate_tone(kSamples, kFs, 4'945.3125, 0.35);

    std::mt19937 rng(42U);
    std::normal_distribution<double> noise(0.0, 0.04);
    for (std::size_t i = 0; i < kSamples; ++i) {
        signal[i] += interferer[i] + noise(rng);
    }

    const double target_power = my_project::dsp::goertzel_power(signal, kFs, 1'640.625);
    const double interferer_power = my_project::dsp::goertzel_power(signal, kFs, 4'945.3125);
    EXPECT_GT(target_power, interferer_power * 6.0);
}

TEST(DspGoertzel, PowerScalesWithAmplitudeSquareLaw) {
    constexpr double kFs = 48'000.0;
    constexpr std::size_t kSamples = 2'048;
    constexpr double kFreq = 2'062.5;  // exactly on DFT bin for 2048 samples at 48kHz

    const auto low_amp = my_project::dsp::generate_tone(kSamples, kFs, kFreq, 0.5);
    const auto high_amp = my_project::dsp::generate_tone(kSamples, kFs, kFreq, 1.0);

    const double low_power = my_project::dsp::goertzel_power(low_amp, kFs, kFreq);
    const double high_power = my_project::dsp::goertzel_power(high_amp, kFs, kFreq);
    const double scale = high_power / low_power;

    EXPECT_NEAR(scale, 4.0, 0.25);
}
