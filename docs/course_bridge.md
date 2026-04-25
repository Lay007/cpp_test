# Course bridge: cpp-dsp-showcase ↔ zynq-sdr-course

`cpp-dsp-showcase` can be used as the **C++ DSP backend track** for the Zynq SDR course:

- course repository: <https://github.com/Lay007/zynq-sdr-course>
- C++ DSP backend: <https://github.com/Lay007/cpp-dsp-showcase>

## Role in the course

The course explains DSP concepts from theory to SDR hardware. This repository provides a compact implementation layer where students can:

1. inspect reference C++ DSP kernels;
2. run deterministic tests;
3. generate visual validation artifacts;
4. benchmark performance;
5. compare floating-point C++ with MATLAB/Simulink and future FPGA-oriented implementations.

## Suggested learning path

| Course stage | Course topic | cpp-dsp-showcase activity |
|---|---|---|
| 1 | DSP tooling overview | build project, run tests, inspect reports |
| 2 | Signals and spectra | generate tones, RMS, frequency response |
| 3 | FIR filtering | design and validate windowed-sinc FIR |
| 4 | Tone detection | run Goertzel detector and plot detection curve |
| 5 | Delay estimation | study GCC-PHAT and benchmark current DFT baseline |
| 6 | Resampling | analyze L/M resampler flow |
| 7 | SDR capture | process recorded IQ files from HDSDR / rtl-sdr |
| 8 | Hardware path | compare C++ baseline with Simulink and FPGA candidates |

## Lab idea: from model to C++ backend

### Goal

Take a simple course signal-processing experiment and reproduce it in C++.

### Example flow

1. Generate or record a tone signal.
2. Analyze it in MATLAB / Simulink / GNU Radio.
3. Run equivalent C++ DSP code from this repository.
4. Compare numerical results and plots.
5. Discuss implementation trade-offs.

## Lab idea: rtl-sdr / HDSDR recording to C++ analysis

### Input

A recorded IQ file from HDSDR or another SDR tool.

### Processing path

```text
rtl-sdr / HDSDR recording
        ↓
IQ file
        ↓
C++ reader / converter
        ↓
FIR filtering / tone detection / delay estimation
        ↓
plots + benchmark report
```

### Learning outcome

Students see the bridge between:

- real RF capture;
- desktop DSP analysis;
- production-style C++ implementation;
- future FPGA acceleration.

## Recommended future additions

- `examples/iq_file_analysis.cpp`
- `docs/iq_file_format.md`
- `tools/convert_ci16_to_float.py`
- benchmark comparison: Python / MATLAB / C++ / FPGA
- fixed-point version of selected kernels
- Vivado / Vitis HLS candidate mapping notes

## Engineering message

The important point is not only that the algorithms work. The important point is that each algorithm has a visible path:

```text
Theory → MATLAB/Simulink model → C++ reference → tests → benchmark → FPGA candidate
```

That makes this repository a practical companion to the SDR course rather than just a standalone code sample.
