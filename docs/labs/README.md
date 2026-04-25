# DSP / SDR Labs

This directory contains a practical lab track that connects the DSP kernels in this repository to SDR-oriented exercises.

## Lab map

| Lab | Topic | Primary outcome |
|---|---|---|
| [Lab 01](lab01_tone_iq_analysis.md) | Tone to IQ to C++ analysis | basic file-based DSP pipeline |
| [Lab 02](lab02_fft_vs_goertzel.md) | FFT vs Goertzel | broad-spectrum vs targeted detection |
| [Lab 03](lab03_gcc_phat_delay_estimation.md) | GCC-PHAT | integer delay estimation |
| [Lab 04](lab04_fir_time_vs_fft.md) | FIR time-domain vs FFT | complexity-driven implementation choice |
| [Lab 05](lab05_resampling_polyphase.md) | Resampling and polyphase concepts | SDR sample-rate conversion |
| [Lab 06](lab06_real_sdr_signal_analysis.md) | Real SDR signal analysis | recorded RF to C++ backend workflow |

## Recommended order

```text
Lab 01 -> Lab 02 -> Lab 03 -> Lab 04 -> Lab 05 -> Lab 06
```

## Expected artifacts

Helper scripts under `tools/` generate plots and sample inputs for several labs.

- generated plots: `docs/assets/`
- generated reports: `build/reports/`
- generated sample IQ data: repository-specific data directories as needed

Large SDR recordings should stay out of version control.
