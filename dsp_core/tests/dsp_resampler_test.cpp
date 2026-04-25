#include "dsp_core/dsp.hpp"

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <numeric>

#include <gtest/gtest.h>

namespace {

double mean(const dsp_core::dsp::Signal& signal) {
    if (signal.empty()) {
        return 0.0;
    }
    return std::accumulate(signal.begin(), signal.end(), 0.0) / static_cast<double>(signal.size());
}

}  // namespace

TEST(DspResampler, ProducesExpectedLengthAndPreservesMainTone) {
    constexpr double kFsIn = 48'000.0;
    constexpr std::size_t kInputSamples = 2'048;
    constexpr int kL = 3;
    constexpr int kM = 2;

    const auto input = dsp_core::dsp::generate_tone(kInputSamples, kFsIn, 1'000.0, 0.9);
    const auto output = dsp_core::dsp::resample_rational(input, kL, kM, 127, 0.95);

    const std::size_t expected = static_cast<std::size_t>(
        std::ceil(static_cast<double>(kInputSamples) * static_cast<double>(kL) /
                  static_cast<double>(kM)));
    ASSERT_EQ(output.size(), expected);

    const double kFsOut = kFsIn * static_cast<double>(kL) / static_cast<double>(kM);
    const auto analysis = dsp_core::dsp::Signal(
        output.begin() + static_cast<std::ptrdiff_t>(80),
        output.end() - static_cast<std::ptrdiff_t>(80));
    const double target_power = dsp_core::dsp::goertzel_power(analysis, kFsOut, 1'000.0);
    const double spur_power = dsp_core::dsp::goertzel_power(analysis, kFsOut, 11'000.0);
    EXPECT_GT(target_power, spur_power * 40.0);
}

TEST(DspResampler, MaintainsDcLevelInSteadyState) {
    constexpr int kL = 5;
    constexpr int kM = 4;
    const dsp_core::dsp::Signal input(400, 1.0);
    const auto output = dsp_core::dsp::resample_rational(input, kL, kM, 127, 0.9);

    ASSERT_GT(output.size(), 160U);
    const auto steady_state = dsp_core::dsp::Signal(
        output.begin() + static_cast<std::ptrdiff_t>(80),
        output.end() - static_cast<std::ptrdiff_t>(80));
    EXPECT_NEAR(mean(steady_state), 1.0, 0.03);
}
