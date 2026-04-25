#include "dsp_core/dsp.hpp"

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <numeric>

#include <gtest/gtest.h>

namespace {

dsp_core::dsp::Signal aligned_view(const dsp_core::dsp::Signal& signal,
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
    const auto taps = dsp_core::dsp::design_lowpass_fir(127, 0.08);
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

    const auto low = dsp_core::dsp::generate_tone(kSamples, kFs, 1'500.0, 1.0);
    const auto high = dsp_core::dsp::generate_tone(kSamples, kFs, 7'500.0, 1.0);
    dsp_core::dsp::Signal mixed(kSamples, 0.0);
    for (std::size_t i = 0; i < kSamples; ++i) {
        mixed[i] = low[i] + high[i];
    }

    const auto taps = dsp_core::dsp::design_lowpass_fir(127, 3'000.0 / kFs);
    const auto filtered = dsp_core::dsp::convolve(mixed, taps);
    const std::size_t delay = (taps.size() - 1U) / 2U;
    const auto aligned = aligned_view(filtered, delay, kSamples);
    ASSERT_FALSE(aligned.empty());

    const double low_power = dsp_core::dsp::goertzel_power(aligned, kFs, 1'500.0);
    const double high_power = dsp_core::dsp::goertzel_power(aligned, kFs, 7'500.0);
    const double ratio = low_power / std::max(high_power, 1e-12);

    EXPECT_GT(ratio, 100.0);

    const double passband = dsp_core::dsp::frequency_response_magnitude(taps, 1'500.0 / kFs);
    const double stopband = dsp_core::dsp::frequency_response_magnitude(taps, 10'000.0 / kFs);
    EXPECT_GT(passband, 0.85);
    EXPECT_LT(stopband, 0.1);
}

TEST(DspFir, FftOverlapSaveMatchesDirectConvolution) {
    constexpr double kFs = 48'000.0;
    constexpr std::size_t kSamples = 2'048;

    const auto tone_a = dsp_core::dsp::generate_tone(kSamples, kFs, 1'200.0, 0.7, 0.1);
    const auto tone_b = dsp_core::dsp::generate_tone(kSamples, kFs, 6'100.0, 0.2, 0.4);

    dsp_core::dsp::Signal mixed(kSamples, 0.0);
    for (std::size_t index = 0; index < kSamples; ++index) {
        mixed[index] = tone_a[index] + tone_b[index];
    }

    const auto taps = dsp_core::dsp::design_lowpass_fir(127, 4'000.0 / kFs);
    const auto reference = dsp_core::dsp::convolve(mixed, taps);
    const auto fft_based = dsp_core::dsp::convolve_fft_overlap_save(mixed, taps, 256);

    ASSERT_EQ(reference.size(), fft_based.size());
    for (std::size_t index = 0; index < reference.size(); ++index) {
        EXPECT_NEAR(reference[index], fft_based[index], 1e-9);
    }
}
