#include "dsp_core/dsp.hpp"

#include <gtest/gtest.h>

TEST(DspApiValidation, GenerateToneRejectsOutOfRangeFrequency) {
    EXPECT_THROW(dsp_core::dsp::generate_tone(128, 48'000.0, 24'000.0), std::invalid_argument);
    EXPECT_THROW(dsp_core::dsp::generate_tone(128, -1.0, 1'000.0), std::invalid_argument);
}

TEST(DspApiValidation, GoertzelBatchValidatesInput) {
    EXPECT_THROW(dsp_core::dsp::goertzel_power_batch({}, 48'000.0, {1'000.0}), std::invalid_argument);
    EXPECT_THROW(dsp_core::dsp::goertzel_power_batch({1.0, 2.0}, 48'000.0, {-10.0}), std::invalid_argument);
}

TEST(DspApiValidation, ResamplerValidatesTapCountAndCutoffScale) {
    const dsp_core::dsp::Signal input(64, 1.0);
    EXPECT_THROW(dsp_core::dsp::resample_rational(input, 2, 1, 32, 0.9), std::invalid_argument);
    EXPECT_THROW(dsp_core::dsp::resample_rational(input, 2, 1, 63, 0.0), std::invalid_argument);
}
