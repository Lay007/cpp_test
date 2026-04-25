#include "my_project/dsp.hpp"

#include <algorithm>
#include <cmath>
#include <complex>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

#ifdef MY_PROJECT_DSP_ENABLE_AVX2
#include <immintrin.h>
#endif

namespace my_project::dsp {
namespace {
constexpr double kPi = 3.14159265358979323846;
using Complex = std::complex<double>;

double sinc(double x) {
    if (std::abs(x) < 1e-12) return 1.0;
    const double pix = kPi * x;
    return std::sin(pix) / pix;
}

std::size_t next_power_of_two(std::size_t value) {
    std::size_t result = 1;
    while (result < value) result <<= 1U;
    return result;
}

void fft_inplace(std::vector<Complex>& a, bool inverse) {
    const std::size_t n = a.size();
    for (std::size_t i = 1, j = 0; i < n; ++i) {
        std::size_t bit = n >> 1U;
        for (; j & bit; bit >>= 1U) j ^= bit;
        j ^= bit;
        if (i < j) std::swap(a[i], a[j]);
    }
    for (std::size_t len = 2; len <= n; len <<= 1U) {
        const double angle = 2.0 * kPi / static_cast<double>(len) * (inverse ? 1.0 : -1.0);
        const Complex wlen(std::cos(angle), std::sin(angle));
        for (std::size_t i = 0; i < n; i += len) {
            Complex w(1.0, 0.0);
            for (std::size_t j = 0; j < len / 2; ++j) {
                const Complex u = a[i + j];
                const Complex v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (inverse) {
        for (auto& v : a) v /= static_cast<double>(n);
    }
}

std::vector<Complex> dft(std::vector<Complex> input, bool inverse) {
    if ((input.size() & (input.size() - 1U)) == 0U) {
        fft_inplace(input, inverse);
        return input;
    }
    const std::size_t n = input.size();
    std::vector<Complex> output(n, Complex(0.0, 0.0));
    const double direction = inverse ? 1.0 : -1.0;
    const double scale = inverse ? 1.0 / static_cast<double>(n) : 1.0;
    for (std::size_t k = 0; k < n; ++k) {
        Complex sum(0.0, 0.0);
        for (std::size_t t = 0; t < n; ++t) {
            const double angle = direction * 2.0 * kPi * static_cast<double>(k * t) / static_cast<double>(n);
            sum += input[t] * Complex(std::cos(angle), std::sin(angle));
        }
        output[k] = sum * scale;
    }
    return output;
}

#ifdef MY_PROJECT_DSP_ENABLE_AVX2
inline double dot_product_avx2(const double* a, const double* b, std::size_t n) {
    __m256d acc = _mm256_setzero_pd();
    std::size_t i = 0;
    for (; i + 4 <= n; i += 4) {
        const __m256d va = _mm256_loadu_pd(a + i);
        const __m256d vb = _mm256_loadu_pd(b + i);
        acc = _mm256_fmadd_pd(va, vb, acc);
    }
    double tmp[4];
    _mm256_storeu_pd(tmp, acc);
    double sum = tmp[0] + tmp[1] + tmp[2] + tmp[3];
    for (; i < n; ++i) sum += a[i] * b[i];
    return sum;
}
#endif
}  // namespace

Signal convolve(const Signal& signal, const Signal& kernel) {
    if (signal.empty() || kernel.empty()) return {};
    const std::size_t n = signal.size();
    const std::size_t m = kernel.size();
    Signal out(n + m - 1U, 0.0);
#ifdef MY_PROJECT_DSP_ENABLE_AVX2
    std::vector<double> kernel_rev(m);
    std::reverse_copy(kernel.begin(), kernel.end(), kernel_rev.begin());
    for (std::size_t i = 0; i < out.size(); ++i) {
        const std::size_t start = (i < m - 1U) ? 0U : i - (m - 1U);
        const std::size_t end = std::min(i, n - 1U);
        const std::size_t len = end - start + 1U;
        out[i] = dot_product_avx2(&signal[start], &kernel_rev[m - len], len);
    }
#else
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < m; ++j) out[i + j] += signal[i] * kernel[j];
    }
#endif
    return out;
}

Signal convolve_fft_overlap_save(const Signal& signal, const Signal& kernel, std::size_t block_size) {
    if (signal.empty() || kernel.empty()) return {};
    const std::size_t m = kernel.size();
    std::size_t nfft = next_power_of_two(std::max(block_size, 2U * m));
    if (nfft <= m) nfft = next_power_of_two(2U * m);
    const std::size_t step = nfft - m + 1U;

    std::vector<Complex> h_freq(nfft, Complex(0.0, 0.0));
    for (std::size_t i = 0; i < m; ++i) h_freq[i] = Complex(kernel[i], 0.0);
    fft_inplace(h_freq, false);

    Signal out;
    out.reserve(signal.size() + m - 1U);
    std::vector<double> history(m - 1U, 0.0);

    for (std::size_t pos = 0; pos < signal.size();) {
        std::vector<Complex> block(nfft, Complex(0.0, 0.0));
        for (std::size_t i = 0; i < history.size(); ++i) block[i] = Complex(history[i], 0.0);
        const std::size_t take = std::min(step, signal.size() - pos);
        for (std::size_t i = 0; i < take; ++i) block[m - 1U + i] = Complex(signal[pos + i], 0.0);
        auto freq = block;
        fft_inplace(freq, false);
        for (std::size_t i = 0; i < nfft; ++i) freq[i] *= h_freq[i];
        fft_inplace(freq, true);
        for (std::size_t i = m - 1U; i < m - 1U + take; ++i) out.push_back(freq[i].real());
        Signal combined;
        combined.reserve(history.size() + take);
        combined.insert(combined.end(), history.begin(), history.end());
        combined.insert(combined.end(), signal.begin() + static_cast<std::ptrdiff_t>(pos), signal.begin() + static_cast<std::ptrdiff_t>(pos + take));
        if (!history.empty()) {
            std::copy(combined.end() - static_cast<std::ptrdiff_t>(history.size()), combined.end(), history.begin());
        }
        pos += take;
    }

    std::vector<Complex> tail(nfft, Complex(0.0, 0.0));
    for (std::size_t i = 0; i < history.size(); ++i) tail[i] = Complex(history[i], 0.0);
    fft_inplace(tail, false);
    for (std::size_t i = 0; i < nfft; ++i) tail[i] *= h_freq[i];
    fft_inplace(tail, true);
    for (std::size_t i = m - 1U; out.size() < signal.size() + m - 1U && i < nfft; ++i) out.push_back(tail[i].real());
    out.resize(signal.size() + m - 1U);
    return out;
}

Signal generate_tone(std::size_t samples, double sample_rate_hz, double frequency_hz, double amplitude, double phase_rad) {
    if (sample_rate_hz <= 0.0 || frequency_hz <= 0.0 || frequency_hz >= sample_rate_hz / 2.0) throw std::invalid_argument("Invalid sample rate or tone frequency.");
    Signal out(samples, 0.0);
    const double omega = 2.0 * kPi * frequency_hz / sample_rate_hz;
    for (std::size_t n = 0; n < samples; ++n) out[n] = amplitude * std::sin(omega * static_cast<double>(n) + phase_rad);
    return out;
}

double rms(const Signal& signal) {
    if (signal.empty()) return 0.0;
#ifdef MY_PROJECT_DSP_ENABLE_AVX2
    __m256d acc = _mm256_setzero_pd();
    std::size_t i = 0;
    for (; i + 4 <= signal.size(); i += 4) {
        const __m256d v = _mm256_loadu_pd(&signal[i]);
        acc = _mm256_fmadd_pd(v, v, acc);
    }
    double tmp[4];
    _mm256_storeu_pd(tmp, acc);
    double energy = tmp[0] + tmp[1] + tmp[2] + tmp[3];
    for (; i < signal.size(); ++i) energy += signal[i] * signal[i];
#else
    const double energy = std::inner_product(signal.begin(), signal.end(), signal.begin(), 0.0);
#endif
    return std::sqrt(energy / static_cast<double>(signal.size()));
}

std::vector<double> design_lowpass_fir(int taps, double cutoff_cycles_per_sample) {
    if (taps < 7 || taps % 2 == 0) throw std::invalid_argument("FIR taps must be odd and >= 7.");
    if (cutoff_cycles_per_sample <= 0.0 || cutoff_cycles_per_sample >= 0.5) throw std::invalid_argument("Cutoff must be in range (0.0, 0.5).");
    std::vector<double> h(static_cast<std::size_t>(taps), 0.0);
    const int midpoint = taps / 2;
    for (int n = 0; n < taps; ++n) {
        const int m = n - midpoint;
        const double ideal = 2.0 * cutoff_cycles_per_sample * sinc(2.0 * cutoff_cycles_per_sample * static_cast<double>(m));
        const double phase = 2.0 * kPi * static_cast<double>(n) / static_cast<double>(taps - 1);
        const double blackman = 0.42 - 0.5 * std::cos(phase) + 0.08 * std::cos(2.0 * phase);
        h[static_cast<std::size_t>(n)] = ideal * blackman;
    }
    const double sum = std::accumulate(h.begin(), h.end(), 0.0);
    if (std::abs(sum) > 1e-12) for (double& tap : h) tap /= sum;
    return h;
}

double frequency_response_magnitude(const std::vector<double>& taps, double normalized_frequency) {
    if (taps.empty()) throw std::invalid_argument("Taps are empty.");
    if (normalized_frequency < 0.0 || normalized_frequency > 0.5) throw std::invalid_argument("Normalized frequency must be in range [0.0, 0.5].");
    Complex acc(0.0, 0.0);
    for (std::size_t n = 0; n < taps.size(); ++n) {
        const double angle = -2.0 * kPi * normalized_frequency * static_cast<double>(n);
        acc += taps[n] * Complex(std::cos(angle), std::sin(angle));
    }
    return std::abs(acc);
}

double goertzel_power(const Signal& signal, double sample_rate_hz, double target_hz) {
    if (signal.empty()) throw std::invalid_argument("Signal is empty.");
    if (sample_rate_hz <= 0.0 || target_hz <= 0.0 || target_hz >= sample_rate_hz / 2.0) throw std::invalid_argument("Invalid sample rate or target frequency.");
    const std::size_t n = signal.size();
    const std::size_t k_bin = static_cast<std::size_t>(std::llround(static_cast<double>(n) * target_hz / sample_rate_hz));
    const double omega = 2.0 * kPi * static_cast<double>(k_bin) / static_cast<double>(n);
    const double coeff = 2.0 * std::cos(omega);
    double s_prev = 0.0, s_prev2 = 0.0;
    for (double sample : signal) {
        const double s = sample + coeff * s_prev - s_prev2;
        s_prev2 = s_prev;
        s_prev = s;
    }
    return (s_prev2 * s_prev2 + s_prev * s_prev - coeff * s_prev * s_prev2) / static_cast<double>(n);
}

std::vector<double> goertzel_power_batch(const Signal& signal, double sample_rate_hz, const std::vector<double>& target_hz) {
    std::vector<double> out(target_hz.size(), 0.0);
#ifdef MY_PROJECT_DSP_ENABLE_AVX2
    std::size_t k = 0;
    for (; k + 4 <= target_hz.size(); k += 4) {
        double coeff_arr[4];
        for (int lane = 0; lane < 4; ++lane) {
            const double f = target_hz[k + static_cast<std::size_t>(lane)];
            const std::size_t bin = static_cast<std::size_t>(std::llround(static_cast<double>(signal.size()) * f / sample_rate_hz));
            coeff_arr[lane] = 2.0 * std::cos(2.0 * kPi * static_cast<double>(bin) / static_cast<double>(signal.size()));
        }
        const __m256d coeff = _mm256_loadu_pd(coeff_arr);
        __m256d s1 = _mm256_setzero_pd();
        __m256d s2 = _mm256_setzero_pd();
        for (double sample : signal) {
            const __m256d x = _mm256_set1_pd(sample);
            const __m256d s0 = _mm256_sub_pd(_mm256_add_pd(x, _mm256_mul_pd(coeff, s1)), s2);
            s2 = s1;
            s1 = s0;
        }
        const __m256d p = _mm256_sub_pd(_mm256_add_pd(_mm256_mul_pd(s2, s2), _mm256_mul_pd(s1, s1)), _mm256_mul_pd(_mm256_mul_pd(coeff, s1), s2));
        double power[4];
        _mm256_storeu_pd(power, p);
        for (int lane = 0; lane < 4; ++lane) out[k + static_cast<std::size_t>(lane)] = power[lane] / static_cast<double>(signal.size());
    }
    for (; k < target_hz.size(); ++k) out[k] = goertzel_power(signal, sample_rate_hz, target_hz[k]);
#else
    for (std::size_t k = 0; k < target_hz.size(); ++k) out[k] = goertzel_power(signal, sample_rate_hz, target_hz[k]);
#endif
    return out;
}

GccPhatResult estimate_delay_gcc_phat(const Signal& reference, const Signal& delayed, int max_lag) {
    if (reference.empty() || delayed.empty()) throw std::invalid_argument("Signals must not be empty.");
    if (max_lag < 0) throw std::invalid_argument("max_lag must be >= 0.");
    const std::size_t n_fft = next_power_of_two(reference.size() + delayed.size());
    std::vector<Complex> ref(n_fft), del(n_fft);
    for (std::size_t i = 0; i < reference.size(); ++i) ref[i] = Complex(reference[i], 0.0);
    for (std::size_t i = 0; i < delayed.size(); ++i) del[i] = Complex(delayed[i], 0.0);
    fft_inplace(ref, false);
    fft_inplace(del, false);
    std::vector<Complex> cross(n_fft);
    for (std::size_t i = 0; i < n_fft; ++i) {
        const Complex product = del[i] * std::conj(ref[i]);
        const double mag = std::abs(product);
        if (mag > 1e-12) cross[i] = product / mag;
    }
    fft_inplace(cross, true);
    int best_lag = 0;
    double best_score = -std::numeric_limits<double>::infinity();
    for (std::size_t i = 0; i < cross.size(); ++i) {
        const int lag = (i <= cross.size() / 2U) ? static_cast<int>(i) : static_cast<int>(i) - static_cast<int>(cross.size());
        if (std::abs(lag) > max_lag) continue;
        if (cross[i].real() > best_score) {
            best_score = cross[i].real();
            best_lag = lag;
        }
    }
    return {best_lag, best_score};
}

Signal resample_rational(const Signal& input, int up_factor, int down_factor, int filter_taps, double cutoff_scale) {
    if (input.empty()) return {};
    if (up_factor <= 0 || down_factor <= 0) throw std::invalid_argument("Resampling factors must be > 0.");
    const double cutoff = 0.5 / static_cast<double>(std::max(up_factor, down_factor)) * cutoff_scale;
    auto anti_alias = design_lowpass_fir(filter_taps, cutoff);
    for (double& tap : anti_alias) tap *= static_cast<double>(up_factor);
    Signal upsampled(input.size() * static_cast<std::size_t>(up_factor), 0.0);
    for (std::size_t i = 0; i < input.size(); ++i) upsampled[i * static_cast<std::size_t>(up_factor)] = input[i];
    const Signal filtered = convolve(upsampled, anti_alias);
    const std::size_t expected = static_cast<std::size_t>(std::ceil(static_cast<double>(input.size()) * up_factor / down_factor));
    Signal output;
    output.reserve(expected);
    const std::size_t delay = static_cast<std::size_t>((filter_taps - 1) / 2);
    for (std::size_t i = 0; i < expected; ++i) {
        const std::size_t index = delay + i * static_cast<std::size_t>(down_factor);
        if (index >= filtered.size()) break;
        output.push_back(filtered[index]);
    }
    return output;
}
}  // namespace my_project::dsp
