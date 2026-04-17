#include "my_project/dsp.hpp"

#include <algorithm>
#include <cmath>
#include <complex>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace my_project::dsp {

namespace {

constexpr double kPi = 3.14159265358979323846;

double sinc(double x) {
    if (std::abs(x) < 1e-12) {
        return 1.0;
    }
    const double pix = kPi * x;
    return std::sin(pix) / pix;
}

std::size_t next_power_of_two(std::size_t value) {
    std::size_t result = 1;
    while (result < value) {
        result <<= 1U;
    }
    return result;
}

std::vector<std::complex<double>> dft(const std::vector<std::complex<double>>& input, bool inverse) {
    const std::size_t n = input.size();
    std::vector<std::complex<double>> output(n, std::complex<double>(0.0, 0.0));
    const double direction = inverse ? 1.0 : -1.0;
    const double scale = inverse ? 1.0 / static_cast<double>(n) : 1.0;

    for (std::size_t k = 0; k < n; ++k) {
        std::complex<double> sum(0.0, 0.0);
        for (std::size_t t = 0; t < n; ++t) {
            const double angle = direction * 2.0 * kPi * static_cast<double>(k * t) /
                                 static_cast<double>(n);
            sum += input[t] * std::complex<double>(std::cos(angle), std::sin(angle));
        }
        output[k] = sum * scale;
    }

    return output;
}

}  // namespace

Signal convolve(const Signal& signal, const Signal& kernel) {
    if (signal.empty() || kernel.empty()) {
        return {};
    }

    Signal out(signal.size() + kernel.size() - 1U, 0.0);
    for (std::size_t i = 0; i < signal.size(); ++i) {
        for (std::size_t j = 0; j < kernel.size(); ++j) {
            out[i + j] += signal[i] * kernel[j];
        }
    }
    return out;
}

Signal generate_tone(std::size_t samples,
                     double sample_rate_hz,
                     double frequency_hz,
                     double amplitude,
                     double phase_rad) {
    if (sample_rate_hz <= 0.0 || frequency_hz <= 0.0 || frequency_hz >= sample_rate_hz / 2.0) {
        throw std::invalid_argument("Invalid sample rate or tone frequency.");
    }

    Signal out(samples, 0.0);
    const double omega = 2.0 * kPi * frequency_hz / sample_rate_hz;
    for (std::size_t n = 0; n < samples; ++n) {
        out[n] = amplitude * std::sin(omega * static_cast<double>(n) + phase_rad);
    }
    return out;
}

double rms(const Signal& signal) {
    if (signal.empty()) {
        return 0.0;
    }

    const double energy = std::inner_product(signal.begin(), signal.end(), signal.begin(), 0.0);
    return std::sqrt(energy / static_cast<double>(signal.size()));
}

std::vector<double> design_lowpass_fir(int taps, double cutoff_cycles_per_sample) {
    if (taps < 7 || taps % 2 == 0) {
        throw std::invalid_argument("FIR taps must be odd and >= 7.");
    }
    if (cutoff_cycles_per_sample <= 0.0 || cutoff_cycles_per_sample >= 0.5) {
        throw std::invalid_argument("Cutoff must be in range (0.0, 0.5).");
    }

    std::vector<double> h(static_cast<std::size_t>(taps), 0.0);
    const int midpoint = taps / 2;
    for (int n = 0; n < taps; ++n) {
        const int m = n - midpoint;
        const double ideal = 2.0 * cutoff_cycles_per_sample *
                             sinc(2.0 * cutoff_cycles_per_sample * static_cast<double>(m));
        const double phase = 2.0 * kPi * static_cast<double>(n) / static_cast<double>(taps - 1);
        const double blackman = 0.42 - 0.5 * std::cos(phase) + 0.08 * std::cos(2.0 * phase);
        h[static_cast<std::size_t>(n)] = ideal * blackman;
    }

    const double sum = std::accumulate(h.begin(), h.end(), 0.0);
    if (std::abs(sum) > 1e-12) {
        for (double& tap : h) {
            tap /= sum;
        }
    }
    return h;
}

double frequency_response_magnitude(const std::vector<double>& taps, double normalized_frequency) {
    if (taps.empty()) {
        throw std::invalid_argument("Taps are empty.");
    }
    if (normalized_frequency < 0.0 || normalized_frequency > 0.5) {
        throw std::invalid_argument("Normalized frequency must be in range [0.0, 0.5].");
    }

    std::complex<double> acc(0.0, 0.0);
    for (std::size_t n = 0; n < taps.size(); ++n) {
        const double angle = -2.0 * kPi * normalized_frequency * static_cast<double>(n);
        acc += taps[n] * std::complex<double>(std::cos(angle), std::sin(angle));
    }
    return std::abs(acc);
}

double goertzel_power(const Signal& signal, double sample_rate_hz, double target_hz) {
    if (signal.empty()) {
        throw std::invalid_argument("Signal is empty.");
    }
    if (sample_rate_hz <= 0.0 || target_hz <= 0.0 || target_hz >= sample_rate_hz / 2.0) {
        throw std::invalid_argument("Invalid sample rate or target frequency.");
    }

    const std::size_t n = signal.size();
    const double k_float = static_cast<double>(n) * target_hz / sample_rate_hz;
    const std::size_t k_bin = static_cast<std::size_t>(std::llround(k_float));
    const double omega = 2.0 * kPi * static_cast<double>(k_bin) / static_cast<double>(n);
    const double coeff = 2.0 * std::cos(omega);

    double s_prev = 0.0;
    double s_prev2 = 0.0;
    for (const double sample : signal) {
        const double s = sample + coeff * s_prev - s_prev2;
        s_prev2 = s_prev;
        s_prev = s;
    }

    const double power = s_prev2 * s_prev2 + s_prev * s_prev - coeff * s_prev * s_prev2;
    return power / static_cast<double>(n);
}

GccPhatResult estimate_delay_gcc_phat(const Signal& reference,
                                      const Signal& delayed,
                                      int max_lag) {
    if (reference.empty() || delayed.empty()) {
        throw std::invalid_argument("Signals must not be empty.");
    }
    if (max_lag < 0) {
        throw std::invalid_argument("max_lag must be >= 0.");
    }

    const std::size_t n_fft = next_power_of_two(reference.size() + delayed.size());
    std::vector<std::complex<double>> ref_freq_input(n_fft, std::complex<double>(0.0, 0.0));
    std::vector<std::complex<double>> del_freq_input(n_fft, std::complex<double>(0.0, 0.0));
    for (std::size_t i = 0; i < reference.size(); ++i) {
        ref_freq_input[i] = std::complex<double>(reference[i], 0.0);
    }
    for (std::size_t i = 0; i < delayed.size(); ++i) {
        del_freq_input[i] = std::complex<double>(delayed[i], 0.0);
    }

    const auto ref_freq = dft(ref_freq_input, false);
    const auto del_freq = dft(del_freq_input, false);

    std::vector<std::complex<double>> cross_spectrum(n_fft, std::complex<double>(0.0, 0.0));
    for (std::size_t i = 0; i < n_fft; ++i) {
        const std::complex<double> product = del_freq[i] * std::conj(ref_freq[i]);
        const double magnitude = std::abs(product);
        if (magnitude > 1e-12) {
            cross_spectrum[i] = product / magnitude;
        }
    }

    const auto correlation = dft(cross_spectrum, true);

    int best_lag = 0;
    double best_score = -std::numeric_limits<double>::infinity();
    for (std::size_t i = 0; i < correlation.size(); ++i) {
        const int lag = (i <= correlation.size() / 2U)
                            ? static_cast<int>(i)
                            : static_cast<int>(i) - static_cast<int>(correlation.size());
        if (std::abs(lag) > max_lag) {
            continue;
        }

        const double score = correlation[i].real();
        if (score > best_score) {
            best_score = score;
            best_lag = lag;
        }
    }

    return {best_lag, best_score};
}

Signal resample_rational(const Signal& input,
                         int up_factor,
                         int down_factor,
                         int filter_taps,
                         double cutoff_scale) {
    if (input.empty()) {
        return {};
    }
    if (up_factor <= 0 || down_factor <= 0) {
        throw std::invalid_argument("Resampling factors must be > 0.");
    }
    if (filter_taps < 7 || filter_taps % 2 == 0) {
        throw std::invalid_argument("filter_taps must be odd and >= 7.");
    }
    if (cutoff_scale <= 0.0 || cutoff_scale > 1.0) {
        throw std::invalid_argument("cutoff_scale must be in range (0.0, 1.0].");
    }

    const double cutoff = 0.5 / static_cast<double>(std::max(up_factor, down_factor)) *
                          cutoff_scale;
    auto anti_alias = design_lowpass_fir(filter_taps, cutoff);
    for (double& tap : anti_alias) {
        tap *= static_cast<double>(up_factor);
    }

    Signal upsampled(input.size() * static_cast<std::size_t>(up_factor), 0.0);
    for (std::size_t i = 0; i < input.size(); ++i) {
        upsampled[i * static_cast<std::size_t>(up_factor)] = input[i];
    }

    const Signal filtered = convolve(upsampled, anti_alias);
    const std::size_t expected_length = static_cast<std::size_t>(
        std::ceil(static_cast<double>(input.size()) * static_cast<double>(up_factor) /
                  static_cast<double>(down_factor)));

    Signal output;
    output.reserve(expected_length);
    const std::size_t delay = static_cast<std::size_t>((filter_taps - 1) / 2);
    for (std::size_t i = 0; i < expected_length; ++i) {
        const std::size_t index = delay + i * static_cast<std::size_t>(down_factor);
        if (index >= filtered.size()) {
            break;
        }
        output.push_back(filtered[index]);
    }
    return output;
}

}  // namespace my_project::dsp
