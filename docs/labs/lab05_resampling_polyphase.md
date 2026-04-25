# Lab 05 — Rational resampling and polyphase thinking

## Purpose

This lab introduces rational sample-rate conversion:

```text
Fs_in → Fs_out = Fs_in · L / M
```

It connects directly to SDR systems, where different blocks often operate at different sample rates.

Typical use cases:

- SDR front-end decimation;
- audio-rate conversion;
- channelizer preprocessing;
- FPGA streaming pipelines;
- matching ADC/DAC rates to DSP algorithm rates.

## Learning outcomes

After this lab, a student should be able to:

- explain rational resampling with factors `L/M`;
- understand why interpolation must be followed by filtering;
- understand why decimation requires anti-alias filtering;
- describe the upsample-filter-decimate flow;
- understand the motivation for polyphase filters;
- connect resampling to SDR and FPGA pipelines.

## Basic flow

```text
Input x[n]
   ↓
Upsample by L
   ↓
Low-pass FIR interpolation filter
   ↓
Decimate by M
   ↓
Output y[n]
```

## Why filtering is necessary

Upsampling inserts zeros and creates spectral images. Decimation can create aliasing. The FIR filter suppresses unwanted images and prevents aliasing before rate reduction.

## Practical comparison

| Approach | Strength | Weakness |
|---|---|---|
| Naive upsample-filter-decimate | simple and easy to verify | inefficient for large L/M |
| Polyphase resampler | efficient and streaming-friendly | more complex implementation |
| Fixed-point polyphase | FPGA/embedded-ready | requires scaling analysis |
| Vendor DSP IP | production speed | less transparent for learning |

## Suggested experiment

Use a two-tone signal and resample it by `L/M = 3/2`.

| Parameter | Value |
|---|---:|
| Input sample rate | 8000 Hz |
| Output sample rate | 12000 Hz |
| L | 3 |
| M | 2 |
| Tone 1 | 700 Hz |
| Tone 2 | 1800 Hz |

## Suggested command

```bash
pip install numpy matplotlib
python tools/generate_lab05_resampling.py
```

Output:

```text
docs/assets/lab05_resampling_polyphase.png
```

## Course connection

This lab is a bridge between algorithmic DSP and SDR system design:

1. simulate a signal at one sample rate;
2. convert it to another sample rate;
3. inspect time-domain and spectral behavior;
4. discuss CPU implementation;
5. discuss FPGA polyphase implementation.

## FPGA / Zynq discussion

Potential hardware mapping:

| Block | FPGA candidate? | Notes |
|---|---|---|
| Interpolator | yes | zero insertion can be implicit |
| FIR filter | yes | DSP slices / polyphase branches |
| Decimator | yes | output enable / rate control |
| Polyphase bank | yes | efficient streaming structure |
| Fixed-point scaling | mandatory | avoid overflow / preserve SNR |

## Complexity discussion

| Implementation | Computational cost | Comment |
|---|---:|---|
| Naive resampler | high | many operations on inserted zeros |
| Polyphase | lower | computes only required output phases |
| SIMD polyphase | lower CPU time | platform-specific |
| FPGA polyphase | deterministic | resource/latency planning required |

## Extension tasks

- implement a polyphase resampler in C++;
- compare naive and polyphase output numerically;
- add fixed-point coefficient quantization;
- benchmark different `L/M` ratios;
- add SDR decimation example;
- compare with MATLAB `resample` or GNU Radio rational resampler;
- map FIR branches to FPGA DSP slices.

## Engineering message

The main design lesson is:

```text
Sample-rate conversion is not just changing the number of samples.
It is filtering plus rate control.
```

A good SDR pipeline treats resampling as a first-class DSP block with validation, complexity analysis, and hardware mapping strategy.
