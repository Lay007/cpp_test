# cpp-dsp-showcase

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](#)
[![CMake](https://img.shields.io/badge/build-CMake-informational)](#)
[![Tests](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

A compact but serious **C++ DSP showcase** focused on practical signal-processing kernels, reproducible verification, and cross-platform engineering discipline.

---

## DSP modules overview

![DSP flow](docs/assets/dsp_modules_flow.svg)

---

## Visual validation artifacts

### FIR frequency response

![FIR](docs/assets/fir_response.png)

### Goertzel tone detection

![Goertzel](docs/assets/goertzel_detection.png)

### GCC-PHAT delay estimation

![GCC](docs/assets/gcc_phat_delay.png)

---

## Algorithm engineering table

| Algorithm | Validation | Complexity | Optimization target |
|---|---|---|---|
| FIR (windowed-sinc) | unit tests + freq response | O(N·M) | SIMD / FFT convolution |
| Convolution | impulse + RMS checks | O(N·M) | overlap-save FFT |
| Goertzel | tone detection tests | O(N) | vectorization |
| GCC-PHAT | delay accuracy tests | O(N²) (DFT) | FFT-based cross-spectrum |
| Resampler L/M | length + spectral checks | O(N·M) | polyphase filters |

---

## Engineering decisions

This repository intentionally prioritizes **clarity and correctness over premature optimization**.

Key design choices:

- **Readable DSP kernels** instead of opaque optimized code
- **Naive DFT baseline for GCC-PHAT** to make algorithm structure explicit
- **Time-domain FIR** as a reference implementation before FFT acceleration
- **Strict test coverage** before performance tuning
- **Separation of concerns**: DSP / tests / benchmarks / reports

Trade-offs:

- Lower raw performance in exchange for transparency
- Explicit optimization roadmap (SIMD, FFT, fixed-point)

This makes the project ideal as a **portfolio artifact and engineering discussion base**.

---

## Highlights

- modern **C++17** codebase
- **CMake + CTest + GoogleTest** workflow
- CI on **Linux** and **Windows**
- correctness-first implementation with clear optimization headroom
- benchmark reporting for performance-oriented iteration

---

## License

MIT
