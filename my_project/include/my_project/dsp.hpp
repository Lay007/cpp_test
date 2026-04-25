#pragma once

#include <cstddef>
#include <vector>

namespace my_project::dsp {

using Signal = std::vector<double>;

struct GccPhatResult {
    int lag_samples{};
    double score{};
};

Signal convolve(const Signal& signal, const Signal& kernel);
Signal convolve_fft_overlap_save(const Signal& signal,
                                 const Signal& kernel,
                                 std::size_t block_size = 1024);

Signal generate_tone(std::size_t samples,
                     double sample_rate_hz,
                     double frequency_hz,
                     double amplitude = 1.0,
                     double phase_rad = 0.0);
double rms(const Signal& signal);

std::vector<double> design_lowpass_fir(int taps, double cutoff_cycles_per_sample);
double frequency_response_magnitude(const std::vector<double>& taps, double normalized_frequency);

double goertzel_power(const Signal& signal, double sample_rate_hz, double target_hz);
std::vector<double> goertzel_power_batch(const Signal& signal,
                                         double sample_rate_hz,
                                         const std::vector<double>& target_hz);

GccPhatResult estimate_delay_gcc_phat(const Signal& reference,
                                      const Signal& delayed,
                                      int max_lag);

Signal resample_rational(const Signal& input,
                         int up_factor,
                         int down_factor,
                         int filter_taps = 127,
                         double cutoff_scale = 0.95);

}  // namespace my_project::dsp
