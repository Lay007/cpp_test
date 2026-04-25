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

    const std::size_t n = signal.size();
    const std::size_t m = kernel.size();
    Signal out(n + m - 1, 0.0);

#ifdef MY_PROJECT_DSP_ENABLE_AVX2
    // reverse kernel once for sliding dot-product
    std::vector<double> kernel_rev(m);
    std::reverse_copy(kernel.begin(), kernel.end(), kernel_rev.begin());

    for (std::size_t i = 0; i < out.size(); ++i) {
        std::size_t start = (i < m - 1) ? 0 : i - (m - 1);
        std::size_t end = std::min(i, n - 1);
        std::size_t len = end - start + 1;

        const double* x_ptr = &signal[start];
        const double* h_ptr = &kernel_rev[m - len];

        out[i] = dot_product_avx2(x_ptr, h_ptr, len);
    }
#else
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < m; ++j) {
            out[i + j] += signal[i] * kernel[j];
        }
    }
#endif

    return out;
}

} // namespace my_project::dsp
