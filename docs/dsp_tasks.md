# DSP Challenge Backlog

This backlog is intentionally non-trivial and targeted at real production signal-processing scenarios.

Russian version: [dsp_tasks_ru.md](dsp_tasks_ru.md)

## 1. Sub-sample TDOA Estimation
- Goal: extend GCC-PHAT with parabolic/interpolated peak fitting to estimate fractional delays.
- Acceptance criteria:
  - Mean absolute error < 0.2 samples on synthetic wideband signals.
  - Stable under SNR from 0 dB to 20 dB.

## 2. Adaptive Noise Canceller (LMS + NLMS + RLS)
- Goal: implement an adaptive filter module and compare convergence speed and steady-state error.
- Acceptance criteria:
  - Reproducible benchmark script with deterministic random seed.
  - Convergence plots and metrics exported to CSV.

## 3. Polyphase Channelizer
- Goal: split a broadband stream into N sub-bands using a PFB architecture.
- Acceptance criteria:
  - Unit tests for perfect reconstruction (analysis + synthesis chain).
  - Throughput benchmark (Msamples/s) in Release mode.

## 4. OFDM Synchronization Block
- Goal: add Schmidl-Cox style coarse synchronization with CFO estimation.
- Acceptance criteria:
  - Correct symbol boundary detection in multipath simulation.
  - CFO estimation error bounded across a documented SNR sweep.

## 5. Real-time Spectral Anomaly Detector
- Goal: streaming STFT + robust baseline estimation (MAD or percentile) + alerting logic.
- Acceptance criteria:
  - False positive/false negative metrics on synthetic injected events.
  - Sliding-window latency budget documented.

## 6. Multi-microphone Beamforming Prototype
- Goal: implement delay-and-sum and MVDR for a small microphone array.
- Acceptance criteria:
  - Directional gain comparison plots for multiple look angles.
  - SNR improvement metrics with interferers from off-axis sources.

## 7. Fixed-point DSP Path
- Goal: provide Q15/Q31 variants for selected kernels (FIR, Goertzel) and quantify quantization error.
- Acceptance criteria:
  - Golden vector tests against double-precision implementation.
  - Saturation/overflow behavior covered by dedicated tests.

## 8. Performance Hardening
- Goal: optimize hot loops with SIMD where available and add cache-friendly block processing.
- Acceptance criteria:
  - Benchmark harness with baseline vs optimized comparison.
  - CI gate that fails on >10% performance regression.
