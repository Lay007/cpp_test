# SIMD acceleration notes

## Goal

The SIMD backend is intentionally optional and enabled with:

```bash
cmake -S . -B build -DDSP_ENABLE_AVX2=ON -DCMAKE_BUILD_TYPE=Release
```

This keeps the default build portable while allowing AVX2/FMA acceleration on suitable x86-64 CPUs.

## Accelerated kernels

| Kernel | SIMD status | Reason |
|---|---|---|
| RMS | AVX2/FMA | independent multiply-add reduction |
| FIR / convolution | AVX2/FMA candidate | sliding dot product structure |
| Goertzel | scalar baseline | recursive dependency limits SIMD gain |
| GCC-PHAT | future FFT/SIMD | transform dominates |
| Resampler | future polyphase/SIMD | FIR branches can be vectorized |

## Why RMS is easy to vectorize

RMS computes a sum of squares:

```text
sum += x[n] * x[n]
```

Each sample can be processed independently, which maps well to SIMD lanes.

## Why Goertzel is not the first SIMD target

Goertzel has a recurrence:

```text
s[n] = x[n] + c*s[n-1] - s[n-2]
```

Each output depends on previous state. That limits straightforward SIMD parallelism. It can still be optimized through multi-frequency batching or block-level parallelism, but it is not the simplest first SIMD kernel.

## FIR direction

Direct FIR can be implemented as a sliding dot product:

```text
y[n] = dot(x_window, h_reversed)
```

This maps naturally to AVX2/FMA. For long filters, FFT-based convolution may still outperform direct SIMD FIR.

## Engineering rule

Use the simplest correct implementation first. Then optimize hot paths with measurable bottlenecks.

```text
scalar reference → SIMD direct kernel → FFT / polyphase / FPGA
```
