# cpp-dsp-showcase

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](#)
[![CMake](https://img.shields.io/badge/build-CMake-informational)](#)
[![Tests](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

A compact but serious **C++ DSP showcase** focused on practical signal-processing kernels, reproducible verification, and cross-platform engineering discipline.

---

## 🎓 Part of SDR course ecosystem

This repository is integrated with:

👉 https://github.com/Lay007/zynq-sdr-course

It acts as a **C++ DSP backend layer** bridging:

- theory (DSP fundamentals)
- modeling (MATLAB / Simulink)
- software (C++)
- hardware (FPGA / SDR)

Detailed integration: `docs/course_bridge.md`

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

---

## Highlights

- modern **C++17** codebase
- **CMake + CTest + GoogleTest** workflow
- CI on **Linux** and **Windows**
- correctness-first implementation

---

## License

MIT
