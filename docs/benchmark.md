# DSP Benchmark Dashboard

This document summarizes performance characteristics of the main DSP kernels.

---

## 🧠 Methodology

Benchmarks are executed using the built-in tool:

```bash
cmake --build build --target benchmark_report
```

or manually:

```bash
./build/my_project_dsp_benchmark --iterations 20 --report build/reports/dsp.md
```

---

## 📊 Visual overview

![Benchmark](assets/benchmark_dashboard.svg)

---

## ⚙️ Performance comparison

| Kernel | Scalar | SIMD (AVX2) | FFT / Advanced |
|--------|--------|------------|----------------|
| FIR (short) | 1x | 5–10x | — |
| FIR (long) | 1x | 4–8x | 10–30x |
| Goertzel | 1x | 2–4x | — |
| RMS | 1x | 4–8x | — |
| GCC-PHAT | 1x | — | 5–15x |

---

## 📈 Typical results (example)

| Method | Time (ms) | Speedup |
|--------|----------|--------|
| FIR SIMD | 18 | 1x |
| FIR FFT | 9 | 2x |
| Goertzel scalar | 80 | 1x |
| Goertzel SIMD | 22 | 3.6x |

---

## 🧭 Engineering takeaway

- SIMD improves constant factors
- FFT improves asymptotic complexity
- optimal method depends on problem size

```text
small problem → SIMD
large problem → FFT
real-time → FPGA
```

---

## 📡 SDR context

In SDR pipelines:

- FIR → channel filtering
- Goertzel → tone detection
- FFT → spectrum / wideband analysis
- GCC-PHAT → synchronization

---

## ⚠️ Notes

Performance depends on:

- CPU architecture
- compiler flags
- memory bandwidth
- signal length

Always benchmark on target hardware.
