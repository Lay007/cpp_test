# cpp-dsp-showcase: C++ DSP Showcase

`cpp-dsp-showcase` is a focused DSP portfolio repository:
- non-trivial signal processing primitives in modern C++;
- reproducible testing with `CMake + CTest + GoogleTest`;
- CI on Linux and Windows with performance checks.

## Implemented DSP blocks

Source:
- `my_project/include/my_project/dsp.hpp`
- `my_project/src/dsp.cpp`

Algorithms:
- Windowed-sinc FIR low-pass design (Blackman window, DC normalization).
- Time-domain convolution.
- Goertzel tone power detector.
- GCC-PHAT delay estimation.
- Rational resampler `L/M` (upsample-filter-decimate).
- Utility helpers for tone generation, RMS, and frequency response.

## Test and perf setup

- Unit tests are auto-discovered via `gtest_discover_tests`.
- Smoke run for DSP demo executable.
- Performance suite available as `ctest -L perf`.
- Benchmark report target writes Markdown report to `build/reports/dsp_performance.md`.

## Quick start

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
ctest --test-dir build -C Release -L perf --output-on-failure
cmake --build build --config Release --target benchmark_report
```

## CI

Workflow file: `.github/workflows/cmake-multi-platform.yml`

Matrix:
- `ubuntu-latest`
- `windows-latest`

CI steps:
- configure and build;
- run all tests;
- run perf label tests (`ctest -L perf`);
- generate and upload benchmark report artifact.

## Repository layout

- `my_project/include/my_project/dsp.hpp` - public DSP API.
- `my_project/src/` - implementations and demo executable.
- `my_project/tests/` - unit tests.
- `my_project/bench/` - performance benchmark executable.
- `docs/dsp_tasks.md` - advanced DSP backlog.
- `docs/perf_report.md` - latest local performance snapshot.

## Advanced DSP backlog

See [docs/dsp_tasks.md](docs/dsp_tasks.md) for non-trivial roadmap items:
- sub-sample TDOA;
- adaptive filters (LMS/NLMS/RLS);
- polyphase channelizer;
- OFDM synchronization;
- beamforming and fixed-point path.
