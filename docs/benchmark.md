# Benchmark Guide

This project ships with a small benchmark executable so that algorithm changes can be evaluated with reproducible numbers instead of assumptions.

## Run the benchmark

Generate the markdown report from CMake:

```bash
cmake --build build --config Release --target benchmark_report
```

Run the benchmark executable directly:

```bash
./build/dsp_core/dsp_core_benchmark --iterations 20 --report build/reports/dsp_performance.md
```

The report is written as a markdown table to make it easy to review in CI artifacts or commit into documentation when needed.

## What is measured

The current benchmark compares representative kernels:

- direct FIR convolution
- FFT overlap-save convolution
- scalar Goertzel evaluation
- batched Goertzel evaluation

These measurements are intended as engineering guidance, not as a vendor-grade microbenchmark suite.

## Reading the results

The generated table includes:

- average execution time in milliseconds
- relative speedup against the direct FIR baseline

Representative example:

| Method | Time (ms) | Speedup |
|---|---:|---:|
| FIR direct | 2.693 | 1.00x |
| FIR overlap-save FFT | 1.816 | 1.48x |
| Goertzel scalar | 0.130 | 20.72x |
| Goertzel batched | 0.485 | 5.55x |

The interpretation matters more than the raw numbers:

```text
short filters -> direct FIR can stay competitive
longer filters -> FFT methods become more attractive
single tone -> scalar Goertzel is simple and efficient
multiple tones -> batching can simplify multi-target analysis
```

## Caveats

Benchmark numbers depend on:

- compiler and flags
- CPU microarchitecture
- memory subsystem behavior
- selected iteration count
- signal size and filter length

Always compare runs on the same hardware and toolchain before drawing conclusions.

## Visual summary

![Benchmark dashboard](assets/benchmark_dashboard.svg)
