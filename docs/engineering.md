# Engineering Notes

## Architecture

The project follows a simple layered layout:

1. Signal generation or input
2. Core DSP kernels
3. Validation through deterministic tests
4. Benchmarking and reporting
5. Documentation and visual artifacts

That separation keeps the reference implementation easy to audit while still leaving room for optimization work.

## Current complexity profile

| Algorithm | Current implementation | Typical complexity | Main bottleneck |
|---|---|---|---|
| FIR convolution | direct time-domain | `O(N * M)` | multiply-accumulate throughput |
| FIR convolution (alt) | FFT overlap-save | `O(N log N)` blocks | transform overhead vs filter length |
| Goertzel | direct recurrence | `O(N)` per target | scalar recurrence |
| GCC-PHAT | FFT-based correlation flow | `O(N log N)` | transform cost |
| Rational resampler | upsample + FIR + decimate | `O(N * M)` | filtering cost |

## Optimization strategy

### Phase 1: low risk

- tighten input validation and error handling
- reduce avoidable allocations
- improve cache locality in hot loops
- keep scalar reference paths easy to verify

### Phase 2: medium risk

- optional AVX2/FMA acceleration
- batched frequency analysis
- target-specific compiler tuning

### Phase 3: high impact

- more aggressive FFT planning choices
- polyphase resampling
- streaming SDR pipeline integration
- fixed-point and FPGA-oriented variants

## Why the code stays readable

This repository is meant to be inspected and taught from.
That means correctness, testability, and traceability are more important than chasing every last percent of performance in the baseline path.

The optimization rule is:

```text
measure first -> optimize the bottleneck -> verify again
```

## Engineering direction

The long-term value of the project is not only in the algorithms themselves, but in the engineering workflow around them:

- define a clear DSP kernel
- validate numerically
- benchmark on real toolchains
- document the result
- prepare the algorithm for downstream hardware or embedded work
