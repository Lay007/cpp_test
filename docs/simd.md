# SIMD Notes

SIMD acceleration is intentionally optional.
The default build stays portable, while x86-64 systems with AVX2/FMA can enable faster kernels when that trade-off is acceptable.

## Enable AVX2

```bash
cmake -S . -B build -DDSP_ENABLE_AVX2=ON -DCMAKE_BUILD_TYPE=Release
```

## Current SIMD posture

| Kernel | Status | Why |
|---|---|---|
| RMS | accelerated | independent multiply-add reduction maps well to AVX2 |
| Direct convolution | accelerated | sliding dot product benefits from vectorized MAC |
| Goertzel batch | partially accelerated | batching multiple targets exposes useful data parallelism |
| GCC-PHAT | not explicitly SIMD tuned | transform structure dominates optimization choices |
| Resampler | baseline only | polyphase restructuring is the next meaningful step |

## What makes a kernel SIMD-friendly

SIMD works best when adjacent samples can be processed independently.
For example, RMS is simply:

```text
sum += x[n] * x[n]
```

That pattern is naturally vectorizable.

## Why not every DSP kernel should be forced into SIMD first

Some algorithms have recursive dependencies or benefit more from an algorithmic change than from lane-level acceleration.
Goertzel is a good example: it has a stateful recurrence, so batching or problem reformulation may matter more than naive vectorization.

The rule here is pragmatic:

```text
algorithmic win first, instruction-level win second
```
