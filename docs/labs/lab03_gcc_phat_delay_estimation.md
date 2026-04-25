# Lab 03 — GCC-PHAT delay estimation

## Purpose

This lab introduces **time delay estimation** using GCC-PHAT — Generalized Cross-Correlation with Phase Transform.

It connects directly to practical DSP and SDR tasks:

- synchronization;
- TDOA localization;
- passive radar / radio monitoring;
- microphone arrays;
- antenna arrays;
- multi-channel SDR capture alignment.

```text
Signal x[n]
    ↓
Delayed signal y[n - d]
    ↓
Cross-spectrum
    ↓
PHAT weighting
    ↓
Inverse transform
    ↓
Delay peak estimate
```

## Learning outcomes

After this lab, a student should be able to:

- explain what time delay estimation is;
- understand the difference between ordinary correlation and PHAT-weighted correlation;
- estimate integer-sample delay;
- interpret the GCC-PHAT peak;
- connect delay estimation to SDR synchronization and TDOA;
- identify why FFT acceleration matters.

## Why GCC-PHAT?

Ordinary cross-correlation is sensitive to signal spectrum and amplitude. PHAT weighting normalizes the cross-spectrum phase:

```text
R_xy[k] = X[k] * conj(Y[k])
G_phat[k] = R_xy[k] / |R_xy[k]|
```

Then the delay is estimated from the peak of the inverse transform.

## Recommended experiment

### Synthetic setup

| Parameter | Value |
|---|---:|
| Samples | 512 |
| True delay | 37 samples |
| Noise | low / moderate |
| Window | Hann |

### Expected result

The GCC-PHAT curve should show a clear peak close to the true delay.

## Suggested command

```bash
pip install numpy matplotlib
python tools/generate_lab03_gcc_phat.py
```

Output:

```text
docs/assets/lab03_gcc_phat_delay.png
```

## Engineering comparison

| Method | Strength | Weakness |
|---|---|---|
| Time-domain correlation | simple and intuitive | expensive for long signals |
| FFT correlation | efficient | requires transform implementation |
| GCC-PHAT | robust delay peak | sensitive to low-SNR edge cases |
| Sub-sample GCC-PHAT | higher accuracy | needs interpolation / fitting |

## Current repository status

`cpp-dsp-showcase` currently contains a readable baseline implementation. This is intentional:

- easier to inspect;
- easier to test;
- useful as a reference;
- clear optimization path toward FFT.

## Complexity discussion

| Implementation | Complexity | Comment |
|---|---:|---|
| Naive DFT-based GCC-PHAT | O(N²) | readable baseline |
| FFT-based GCC-PHAT | O(N log N) | production direction |
| Streaming / block GCC | depends | needed for real-time SDR |

## SDR course connection

This lab fits naturally after spectrum analysis:

1. generate or record two related signals;
2. add or observe a delay;
3. estimate delay using GCC-PHAT;
4. compare Python / MATLAB / C++ results;
5. discuss mapping to FPGA or accelerated CPU implementation.

## FPGA / Zynq discussion

Potential hardware mapping:

| Block | FPGA candidate? | Notes |
|---|---|---|
| FFT / IFFT | yes | vendor IP core |
| Complex multiply | yes | DSP slices |
| Magnitude normalization | yes | fixed-point care required |
| Peak search | yes | streaming reduction |
| Sub-sample interpolation | possible | optional refinement |

## Extension tasks

- implement FFT-based GCC-PHAT in C++;
- add sub-sample parabolic interpolation;
- test several SNR levels;
- test multipath-like reflections;
- compare ordinary correlation vs GCC-PHAT;
- process real two-channel SDR or audio recordings;
- create a TDOA localization mini-project.

## Engineering message

This lab introduces a realistic DSP design decision:

```text
Readable reference first → tested baseline → accelerated FFT version → hardware candidate
```

That mirrors a professional DSP development process.
