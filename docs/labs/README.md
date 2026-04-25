# DSP / SDR labs

This directory contains the practical lab track that connects `cpp-dsp-showcase` with the SDR course.

## Lab map

| Lab | Topic | Main skill |
|---|---|---|
| [Lab 01](lab01_tone_iq_analysis.md) | Tone → IQ → C++ analysis | file-based DSP pipeline |
| [Lab 02](lab02_fft_vs_goertzel.md) | FFT vs Goertzel | spectrum vs targeted detection |
| [Lab 03](lab03_gcc_phat_delay_estimation.md) | GCC-PHAT | delay estimation / TDOA |
| [Lab 04](lab04_fir_time_vs_fft.md) | FIR time-domain vs FFT | complexity-driven design |
| [Lab 05](lab05_resampling_polyphase.md) | Resampling / polyphase | SDR sample-rate conversion |
| [Lab 06](lab06_real_sdr_signal_analysis.md) | Real SDR signal analysis | RF → IQ → C++ backend |

## Recommended order

```text
Lab 01 → Lab 02 → Lab 03 → Lab 04 → Lab 05 → Lab 06
```

## Course-level idea

```text
Theory → MATLAB/Simulink → Python plots → C++ reference → tests → benchmarks → FPGA candidate
```

## Output artifacts

Most labs have helper scripts under `tools/` that generate plots or sample IQ files.

Generated plots are stored in:

```text
docs/assets/
```

Generated binary IQ files are stored in:

```text
data/
```

Large real recordings should not be committed to the repository.
