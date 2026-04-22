# cpp-dsp-showcase

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](#)
[![CMake](https://img.shields.io/badge/build-CMake-informational)](#)
[![Tests](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

A compact but serious **C++ DSP showcase** focused on practical signal-processing kernels, reproducible verification, and cross-platform engineering discipline.

This repository is built to demonstrate more than isolated DSP formulas. It shows how classic signal-processing blocks can be packaged into a modern C++ project with clean builds, automated testing, CI, and benchmark reporting.

## Highlights

- modern **C++17** codebase
- **CMake + CTest + GoogleTest** workflow
- CI on **Linux** and **Windows**
- correctness-first implementation with clear optimization headroom
- benchmark reporting for performance-oriented iteration

## Why this repository matters

Many DSP repositories stop at notebook-level prototypes or disconnected code snippets. `cpp-dsp-showcase` is intentionally positioned as a **portfolio-grade engineering sample**: small enough to review quickly, but substantial enough to demonstrate algorithm design, code organization, verification strategy, and performance awareness.

It is especially useful for:

- C++ DSP portfolio presentation
- interview and technical discussion material
- experimentation with classic DSP primitives
- future expansion toward SDR, audio, radar, and measurement-oriented workflows

## Implemented DSP blocks

### Core source files

- `my_project/include/my_project/dsp.hpp`
- `my_project/src/dsp.cpp`

### Available algorithms and helpers

- **Windowed-sinc FIR low-pass design** using a Blackman window and DC normalization
- **Time-domain convolution**
- **Goertzel tone power detector**
- **GCC-PHAT delay estimation**
- **Rational resampler `L/M`** using upsample-filter-decimate flow
- **Utility helpers** for tone generation, RMS, and frequency-response inspection

## Verification and performance workflow

The repository is structured to validate both correctness and practical usability:

- unit tests are auto-discovered via `gtest_discover_tests`
- a smoke test exercises the demo executable
- a dedicated performance suite is exposed through `ctest -L perf`
- a benchmark target generates a Markdown report at `build/reports/dsp_performance.md`

## Quick start

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
ctest --test-dir build -C Release -L perf --output-on-failure
cmake --build build --config Release --target benchmark_report
```

## Run the demo

```bash
cmake --build build --config Release --target my_project_demo
./build/my_project/my_project_demo
```

The demo prints key runtime indicators such as input RMS, filtered RMS, passband magnitude, stopband magnitude, and resampled signal length.

## Benchmark snapshot

A local performance snapshot is available in [docs/perf_report.md](docs/perf_report.md):

| Benchmark | Samples | Avg time (ms) | Throughput (MSa/s) |
|---|---:|---:|---:|
| FIR 127-tap convolution | 32768 | 2.794 | 11.727 |
| Goertzel power detector | 8192 | 0.044 | 184.380 |
| GCC-PHAT delay estimate | 1024 | 179.096 | 0.006 |

These values are hardware-, compiler-, and configuration-dependent, but they make the repository immediately more useful for discussing optimization priorities.

## CI

Workflow file:

- `.github/workflows/cmake-multi-platform.yml`

### CI matrix

- `ubuntu-latest`
- `windows-latest`

### CI responsibilities

- configure and build the project
- run the full automated test suite
- run performance-labeled tests
- generate and upload the benchmark report artifact

## Repository layout

- `my_project/include/my_project/dsp.hpp` — public DSP API
- `my_project/src/` — implementations and demo executable
- `my_project/tests/` — unit tests
- `my_project/bench/` — performance benchmark executable
- `docs/dsp_tasks.md` — advanced DSP backlog
- `docs/perf_report.md` — local performance snapshot

## Performance notes

A notable current optimization target is the GCC-PHAT path, where the implementation currently uses a naive DFT-based approach. That keeps the algorithm readable while leaving clear room for future FFT-based acceleration.

In other words, the repository is already strong as a correctness-first showcase while still exposing realistic performance engineering opportunities.

## Roadmap

See [docs/dsp_tasks.md](docs/dsp_tasks.md) for the advanced backlog. Current directions include:

- sub-sample TDOA
- adaptive filters (`LMS`, `NLMS`, `RLS`)
- polyphase channelizer
- OFDM synchronization
- beamforming
- fixed-point DSP kernels
- SIMD-oriented optimization and performance hardening

## Future improvements

Potential next steps for making the repository even stronger:

- replace naive DFT paths with FFT-based implementations
- add SIMD-accelerated kernels for selected hot loops
- introduce fixed-point variants for embedded-oriented DSP paths
- add richer benchmark comparisons across platforms and toolchains
- include small visual artifacts such as impulse-response or frequency-response plots

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
