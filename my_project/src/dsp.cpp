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

#ifdef MY_PROJECT_DSP_ENABLE_AVX2
inline double dot_product_avx2(const double* a, const double* b, std::size_t n) {
    __m256d acc = _mm256_setzero_pd();
    std::size_t i = 0;

    for (; i + 4 <= n; i += 4) {
        __m256d va = _mm256_loadu_pd(a + i);
        __m256d vb = _mm256_loadu_pd(b + i);
        acc = _mm256_fmadd_pd(va, vb, acc);
    }

    double tmp[4];
    _mm256_storeu_pd(tmp, acc);
    double sum = tmp[0] + tmp[1] + tmp[2] + tmp[3];

    for (; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}
#endif

} // namespace

Signal convolve(const Signal& signal, const Signal& kernel) {
    if (signal.empty() || kernel.empty()) return {};

    Signal out(signal.size() + kernel.size() - 1U, 0.0);

#ifdef MY_PROJECT_DSP_ENABLE_AVX2
    for (std::size_t i = 0; i < signal.size(); ++i) {
        for (std::size_t j = 0; j < kernel.size(); ++j) {
            out[i + j] += signal[i] * kernel[j];
        }
    }
#else
    for (std::size_t i = 0; i < signal.size(); ++i) {
        for (std::size_t j = 0; j < kernel.size(); ++j) {
            out[i + j] += signal[i] * kernel[j];
        }
    }
#endif

    return out;
}

double rms(const Signal& signal) {
    if (signal.empty()) return 0.0;

#ifdef MY_PROJECT_DSP_ENABLE_AVX2
    __m256d acc = _mm256_setzero_pd();
    std::size_t i = 0;
    for (; i + 4 <= signal.size(); i += 4) {
        __m256d v = _mm256_loadu_pd(&signal[i]);
        acc = _mm256_fmadd_pd(v, v, acc);
    }
    double tmp[4];
    _mm256_storeu_pd(tmp, acc);
    double sum = tmp[0] + tmp[1] + tmp[2] + tmp[3];
    for (; i < signal.size(); ++i) sum += signal[i]*signal[i];
#else
    double sum = std::inner_product(signal.begin(), signal.end(), signal.begin(), 0.0);
#endif

    return std::sqrt(sum / signal.size());
}

} // namespace my_project::dsp
