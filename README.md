# cpp-dsp-showcase

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](#)
[![CMake](https://img.shields.io/badge/build-CMake-informational)](#)
[![Tests](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

A compact but serious **C++ DSP showcase** repository focused on practical signal-processing kernels, reproducible verification, and cross-platform engineering discipline.

This project is meant to demonstrate more than just isolated DSP functions. It shows how core algorithms can be packaged as a small modern C++ codebase with:

- clean CMake-based builds;
- automated unit, smoke, and performance tests;
- Linux and Windows CI;
- benchmark reporting and an extensible roadmap for deeper DSP work.

## Why this repository matters

Many DSP repositories stop at formulas or quick prototypes. `cpp-dsp-showcase` is intentionally positioned as a **portfolio-grade engineering sample**: small enough to inspect quickly, but substantial enough to demonstrate algorithm design, code organization, testability, and performance awareness.

It is especially relevant for:

- C++ DSP development portfolios;
- interview and technical review discussions;
- experimentation with classic signal-processing primitives;
- future expansion toward more advanced SDR/audio/radar-oriented blocks.

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

- unit tests are auto-discovered via `gtest_discover_tests`;
- a smoke test exercises the demo executable;
- a dedicated performance suite is exposed through `ctest -L perf`;
- a benchmark target generates a Markdown performance report in `build/reports/dsp_performance.md`.

## Quick start

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
ctest --test-dir build -C Release -L perf --output-on-failure
cmake --build build --config Release --target benchmark_report
```

## CI

Workflow file:

- `.github/workflows/cmake-multi-platform.yml`

### CI matrix

- `ubuntu-latest`
- `windows-latest`

### CI responsibilities

- configure and build the project;
- run the full automated test suite;
- run performance-labeled tests;
- generate and upload the benchmark report artifact.

## Repository layout

- `my_project/include/my_project/dsp.hpp` — public DSP API
- `my_project/src/` — implementations and demo executable
- `my_project/tests/` — unit tests
- `my_project/bench/` — performance benchmark executable
- `docs/dsp_tasks.md` — advanced DSP backlog
- `docs/perf_report.md` — latest local performance snapshot

## Performance notes

The repository already includes a local performance snapshot in [docs/perf_report.md](docs/perf_report.md). That makes it easier to treat performance as an engineering concern rather than an afterthought.

A notable current optimization target is the GCC-PHAT path, where the implementation currently uses a naive DFT-based approach. This is useful because it keeps the algorithm readable while leaving clear room for future FFT-based acceleration.

## Roadmap

See [docs/dsp_tasks.md](docs/dsp_tasks.md) for the advanced backlog. Current directions include:

- sub-sample TDOA;
- adaptive filters (`LMS`, `NLMS`, `RLS`);
- polyphase channelizer;
- OFDM synchronization;
- beamforming;
- fixed-point DSP kernels;
- performance hardening and SIMD-oriented optimization.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
