#include "my_project/dsp.hpp"

#include <cstdint>

#include <gtest/gtest.h>

namespace {

my_project::dsp::Signal make_wideband_signal(std::size_t size) {
    my_project::dsp::Signal out(size, 0.0);
    uint32_t state = 0x12345678U;
    for (std::size_t i = 0; i < size; ++i) {
        state = 1664525U * state + 1013904223U;
        const double bipolar = (state & 1U) ? 1.0 : -1.0;
        out[i] = bipolar;
    }
    return out;
}

}  // namespace

TEST(DspGccPhat, RecoversIntegerDelay) {
    constexpr std::size_t kSamples = 512;
    constexpr int kDelay = 11;

    const auto reference = make_wideband_signal(kSamples);
    my_project::dsp::Signal delayed(kSamples, 0.0);
    for (std::size_t i = static_cast<std::size_t>(kDelay); i < kSamples; ++i) {
        delayed[i] = reference[i - static_cast<std::size_t>(kDelay)];
    }

    const auto result = my_project::dsp::estimate_delay_gcc_phat(reference, delayed, 32);
    EXPECT_EQ(result.lag_samples, kDelay);
    EXPECT_GT(result.score, 0.1);
}
