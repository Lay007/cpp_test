# Lab 01 — Tone → IQ recording → C++ analysis

## Purpose

This lab connects the SDR course workflow with the C++ DSP showcase repository.

The goal is to move from a simple RF/audio tone experiment to a reproducible C++ analysis pipeline:

```text
Signal source
    ↓
rtl-sdr / HDSDR / simulated IQ
    ↓
ci16 IQ file
    ↓
C++ analysis utility
    ↓
FIR / Goertzel / spectrum indicators
    ↓
report + plots
```

## Learning outcomes

After this lab, a student should be able to:

- understand what an IQ file contains;
- distinguish interleaved `int16` I/Q samples from floating-point DSP vectors;
- convert recorded samples to normalized floating-point values;
- run basic C++ DSP analysis on recorded or simulated data;
- compare C++ results with MATLAB / Simulink / Python / GNU Radio.

## Input signal options

### Option A — simulated signal

Use a generated tone for deterministic first tests. This option does not require RF hardware.

### Option B — rtl-sdr / HDSDR recording

Record a tone-like signal using HDSDR or another SDR tool. Export raw IQ samples in interleaved signed 16-bit format:

```text
I0, Q0, I1, Q1, I2, Q2, ...
```

Recommended filename:

```text
data/lab01_tone_ci16.iq
```

## Suggested parameters

| Parameter | Recommended value |
|---|---:|
| Sample format | complex int16 / ci16 |
| Sample rate | 1.024 MSa/s or lower for first experiments |
| Duration | 1–5 seconds |
| Signal type | single tone or narrowband source |
| Initial analysis window | 8192–65536 complex samples |

## C++ processing flow

1. Read interleaved ci16 samples.
2. Convert to normalized floating-point complex samples.
3. Extract I channel or magnitude for scalar DSP blocks when needed.
4. Estimate RMS.
5. Run Goertzel tone detector for a selected frequency.
6. Optionally apply FIR filtering.
7. Save a small text report.

## Example command

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/my_project/iq_analysis_demo data/lab01_tone_ci16.iq 1024000 100000
```

Arguments:

```text
iq_analysis_demo <file> <sample_rate_hz> <tone_frequency_hz>
```

## Validation checklist

| Check | Expected result |
|---|---|
| File opens correctly | non-zero sample count |
| I/Q scaling | values approximately in [-1, +1] |
| RMS | reasonable non-zero level |
| Goertzel target | visible response near target tone |
| Report | deterministic text output |

## Comparison with course tools

| Tool | Role |
|---|---|
| MATLAB / Simulink | model and reference analysis |
| Python | quick plots and validation |
| GNU Radio | SDR prototyping |
| C++ | production-style DSP backend |
| FPGA / Zynq | acceleration and real-time implementation |

## Extension tasks

- add spectrum estimation;
- add automatic peak search;
- compare Goertzel with FFT bin power;
- add fixed-point scaling discussion;
- stream data from SDR instead of reading a file;
- map FIR / Goertzel blocks to FPGA candidates.

## Engineering discussion

This lab demonstrates the main course message:

```text
Theory → model → recording → C++ reference → validation → hardware candidate
```

The C++ implementation is not just a code exercise. It is a reference backend that helps validate future optimized implementations, including SIMD and FPGA versions.
