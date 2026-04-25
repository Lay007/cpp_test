# cpp-dsp-showcase

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](#)
[![CMake](https://img.shields.io/badge/build-CMake-informational)](#)
[![Tests](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

---

## 🚀 DSP engineering showcase + SDR course backend

This repository is not just a collection of DSP algorithms.

It is a **complete engineering pipeline**:

```text
Signal → SDR → IQ → C++ DSP → analysis → benchmark → FPGA candidate
```

It is tightly integrated with:

👉 https://github.com/Lay007/zynq-sdr-course

---

## 🧠 What this project demonstrates

- DSP fundamentals in real code
- algorithm trade-offs and complexity
- reproducible testing
- performance awareness
- SDR signal processing workflow
- FPGA-ready thinking

---

## 📚 Lab track (core of the project)

👉 See full list: `docs/labs/README.md`

| Lab | Topic |
|---|---|
| Lab 01 | IQ → C++ analysis |
| Lab 02 | FFT vs Goertzel |
| Lab 03 | GCC-PHAT |
| Lab 04 | FIR (time vs FFT) |
| Lab 05 | Resampling (polyphase) |
| Lab 06 | Real SDR signal |

---

## 🧩 DSP modules

![DSP flow](docs/assets/dsp_modules_flow.svg)

---

## 📊 Visual DSP artifacts

| FIR | Goertzel | GCC |
|-----|----------|-----|
| ![](docs/assets/fir_response.png) | ![](docs/assets/goertzel_detection.png) | ![](docs/assets/gcc_phat_delay.png) |

---

## ⚙️ Engineering decisions

- correctness-first implementation
- readable DSP kernels
- naive baselines → optimization roadmap
- explicit trade-offs instead of hidden complexity

---

## 📈 Algorithm overview

| Algorithm | Complexity | Target optimization |
|---|---|---|
| FIR | O(N·M) | SIMD / FFT |
| Goertzel | O(N) | vectorization |
| GCC-PHAT | O(N²) → O(N log N) | FFT |
| Resampler | O(N·M) | polyphase |

---

## 🚀 Quick start

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

---

## 🧪 Generate DSP visuals

```bash
pip install numpy matplotlib
python tools/generate_dsp_plots.py
```

---

## 📡 SDR workflow example

```bash
python tools/generate_lab06_ci16_tone.py
./build/my_project/iq_analysis_demo data/lab06_simulated_ci16.iq 1024000 100000
```

---

## 🧭 Engineering roadmap

- SIMD / AVX optimization
- FFT-based acceleration
- real SDR streaming
- FPGA mapping (Zynq)

---

## 📄 License

MIT
