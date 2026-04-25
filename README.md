# cpp-dsp-showcase

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](#)
[![CMake](https://img.shields.io/badge/build-CMake-informational)](#)
[![CI](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Lay007/cpp-dsp-showcase/actions/workflows/cmake-multi-platform.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

`cpp-dsp-showcase` is a compact, production-style DSP showcase in modern C++.
It is structured as a small engineering portfolio project rather than a single demo:
deterministic kernels, unit tests, benchmark tooling, CI, and supporting SDR course material all live in one repository.

## Highlights

- C++17 DSP library with a compact and installable public API
- Deterministic unit tests for FIR, Goertzel, GCC-PHAT, and rational resampling
- Optional AVX2 acceleration for hot paths
- Benchmark executable plus generated markdown performance reports
- SDR-oriented lab track and supporting plots under `docs/`
- Installable CMake package export for downstream projects

## Implemented modules

| Area | What is included |
|---|---|
| FIR filtering | Windowed-sinc low-pass design, direct convolution, FFT overlap-save convolution |
| Tone detection | Single-frequency and batched Goertzel power estimation |
| Delay estimation | GCC-PHAT baseline for integer delay recovery |
| Sample-rate conversion | Rational `L/M` resampler with anti-alias filtering |
| Tooling | Demo apps, benchmarks, unit tests, generated documentation assets |

## Build

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DDSP_ENABLE_AVX2=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --build-config Release --output-on-failure
```

Generate the benchmark markdown report:

```bash
cmake --build build --config Release --target benchmark_report
```

Install the library and exported CMake package:

```bash
cmake --install build --config Release --prefix install
```

## Using the library from another CMake project

```cmake
find_package(cpp_dsp_showcase CONFIG REQUIRED)

add_executable(example main.cpp)
target_link_libraries(example PRIVATE dsp_core::dsp)
```

The public header is:

```text
dsp_core/dsp.hpp
```

## Executables

| Target | Purpose |
|---|---|
| `dsp_core_demo` | Smoke-test style DSP demo with FIR and resampling output |
| `iq_analysis_demo` | Read CI16 IQ data and evaluate tone power with Goertzel |
| `dsp_core_benchmark` | Run timing measurements and optionally emit a markdown report |

## Project layout

```text
.
|- dsp_core/
|  |- include/dsp_core/dsp.hpp
|  |- src/
|  |- tests/
|  `- bench/
|- docs/
|  |- assets/
|  `- labs/
|- tools/
|- cmake/
`- .github/workflows/
```

## Documentation

- Engineering notes: [docs/engineering.md](docs/engineering.md)
- SIMD notes: [docs/simd.md](docs/simd.md)
- Benchmark guide: [docs/benchmark.md](docs/benchmark.md)
- SDR course bridge: [docs/course_bridge.md](docs/course_bridge.md)
- Lab track: [docs/labs/README.md](docs/labs/README.md)

## Visuals

### DSP module flow

![DSP flow](docs/assets/dsp_modules_flow.svg)

### Benchmark dashboard

![Benchmark dashboard](docs/assets/benchmark_dashboard.svg)

## Scope and intent

This repository is intentionally positioned between classroom material and production engineering:
the code is small enough to study, but structured enough to demonstrate how DSP kernels should be validated, benchmarked, and packaged in a real C++ codebase.

## License

MIT
