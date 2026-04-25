# cpp-dsp-showcase

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](#)
[![CMake](https://img.shields.io/badge/build-CMake-informational)](#)
[![Tests](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

---

## 🚀 DSP engineering showcase + SDR course backend

```text
Signal → SDR → IQ → C++ DSP → analysis → benchmark → FPGA candidate
```

---

## 📚 Lab track

See: `docs/labs/README.md`

---

## 🧩 DSP modules

![DSP flow](docs/assets/dsp_modules_flow.svg)

---

## 📊 Visual DSP artifacts

### FIR response
![FIR](docs/assets/fir_response.svg)

### Goertzel detection
![Goertzel](docs/assets/goertzel_detection.svg)

### GCC-PHAT delay
![GCC](docs/assets/gcc_phat_delay.svg)

---

## 📊 Performance dashboard

![Benchmark](docs/assets/benchmark_dashboard.svg)

👉 Full report: `docs/benchmark.md`

---

## 🚀 Quick start

```bash
cmake -S . -B build -DDSP_ENABLE_AVX2=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --build build --target benchmark_report
```

---

## 🧭 Engineering roadmap

- SIMD / AVX optimization
- FFT-based acceleration
- SDR streaming
- FPGA mapping

---

## 📄 License

MIT
