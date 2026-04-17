#include "my_project/dsp.hpp"

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <numeric>

#include <gtest/gtest.h>

namespace {

my_project::dsp::Signal aligned_view(const my_project::dsp::Signal& signal,
                                     std::size_t start,
                                     std::size_t count) {
    if (start + count > signal.size()) {
        return {};
    }
    return {signal.begin() + static_cast<std::ptrdiff_t>(start),
            signal.begin() + static_cast<std::ptrdiff_t>(start + count)};
}

}  // namespace

TEST(DspFir, ProducesSymmetricKernelWithUnityDcGain) {
    const auto taps = my_project::dsp::design_lowpass_fir(127, 0.08);
    ASSERT_EQ(taps.size(), 127U);

    for (std::size_t i = 0; i < taps.size() / 2U; ++i) {
        EXPECT_NEAR(taps[i], taps[taps.size() - 1U - i], 1e-12);
    }

    const double dc_gain = std::accumulate(taps.begin(), taps.end(), 0.0);
    EXPECT_NEAR(dc_gain, 1.0, 1e-8);
}

TEST(DspFir, SuppressesOutOfBandTone) {
    constexpr double kFs = 48'000.0;
    constexpr std::size_t kSamples = 4'096;

    const auto low = my_project::dsp::generate_tone(kSamples, kFs, 1'500.0, 1.0);
    const auto high = my_project::dsp::generate_tone(kSamples, kFs, 7'500.0, 1.0);
    my_project::dsp::Signal mixed(kSamples, 0.0);
    for (std::size_t i = 0; i < kSamples; ++i) {
        mixed[i] = low[i] + high[i];
    }

    const auto taps = my_project::dsp::design_lowpass_fir(127, 3'000.0 / kFs);
    const auto filtered = my_project::dsp::convolve(mixed, taps);
    const std::size_t delay = (taps.size() - 1U) / 2U;
    const auto aligned = aligned_view(filtered, delay, kSamples);
    ASSERT_FALSE(aligned.empty());

    const double low_power = my_project::dsp::goertzel_power(aligned, kFs, 1'500.0);
    const double high_power = my_project::dsp::goertzel_power(aligned, kFs, 7'500.0);
    const double ratio = low_power / std::max(high_power, 1e-12);

    EXPECT_GT(ratio, 100.0);

    const double passband = my_project::dsp::frequency_response_magnitude(taps, 1'500.0 / kFs);
    const double stopband = my_project::dsp::frequency_response_magnitude(taps, 10'000.0 / kFs);
    EXPECT_GT(passband, 0.85);
    EXPECT_LT(stopband, 0.1);
}
