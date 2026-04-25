# Engineering deep-dive

## Architecture principles

The project follows a **layered DSP architecture**:

1. Signal generation / input
2. Core DSP kernels
3. Validation layer
4. Benchmarking and reporting

## Performance model

| Algorithm | Current impl | Complexity | Bottleneck |
|----------|-------------|------------|------------|
| FIR | time-domain | O(N·M) | memory + MAC |
| Goertzel | direct | O(N) | scalar loop |
| GCC-PHAT | naive DFT | O(N²) | transform cost |
| Resampler | naive | O(N·M) | filtering |

## Optimization roadmap

### Phase 1 (low risk)
- loop unrolling
- memory alignment
- cache locality improvements

### Phase 2 (medium)
- SIMD (AVX2 / AVX-512 / NEON)
- fused multiply-add usage

### Phase 3 (high impact)
- FFT-based convolution
- FFT-based GCC-PHAT
- polyphase resampling

## SDR integration direction

Future evolution:

- rtl-sdr input
- IQ file processing (int16 complex)
- real-time pipeline

## Why not optimized yet?

Because:

- readability is critical for verification
- baseline must be correct before optimization
- easier to validate step-by-step improvements

This is a deliberate **engineering-first strategy**.
