# Lab 02 — Spectrum analysis: FFT vs Goertzel

## Purpose

This lab compares two common approaches to tone analysis:

1. **FFT spectrum analysis** — broad view of many frequency bins.
2. **Goertzel detector** — efficient targeted detection of one selected frequency.

The lab continues the SDR course bridge started in Lab 01 and shows how a recorded or simulated signal can be analyzed both visually and numerically.

```text
IQ / scalar signal
      ↓
windowing
      ↓
FFT spectrum     Goertzel target bin
      ↓                  ↓
wideband view      narrowband detector
```

## Learning outcomes

After this lab, a student should be able to:

- explain when FFT is preferable to Goertzel;
- explain when Goertzel is preferable to FFT;
- understand frequency bin spacing;
- understand why windowing matters;
- compare spectrum peak location with Goertzel power;
- connect analysis results to SDR recordings.

## Key concepts

### FFT spectrum

The FFT provides a compact way to inspect many frequency bins at once. It is useful when the signal content is unknown or when we need a visual spectrum.

### Goertzel detector

Goertzel is useful when the target frequency is known in advance. It avoids computing the whole spectrum and is often used in tone detection tasks.

## Recommended experiment

### Signal

Use a synthetic tone with weak interference:

| Component | Frequency | Amplitude |
|---|---:|---:|
| Target tone | 1000 Hz | 1.0 |
| Interferer | 2300 Hz | 0.25 |
| Noise | broadband | low |

### Sampling

| Parameter | Value |
|---|---:|
| Sample rate | 8000 Hz |
| Samples | 1024 or 2048 |
| Window | Hann |

## Expected result

- FFT should show a dominant peak near the target tone.
- Goertzel should report high power at the target frequency.
- Goertzel power at non-target frequencies should be lower.

## Suggested commands

Generate demonstration plot:

```bash
pip install numpy matplotlib
python tools/generate_lab02_fft_vs_goertzel.py
```

Output:

```text
docs/assets/lab02_fft_vs_goertzel.png
```

## FFT vs Goertzel comparison

| Criterion | FFT | Goertzel |
|---|---|---|
| Best use case | broad spectrum inspection | known-frequency detection |
| Output | many bins | one bin / selected bins |
| Complexity | O(N log N) | O(N) per target |
| Visualization | excellent | limited |
| Embedded use | possible, heavier | very attractive |
| SDR debugging | excellent | useful for detectors |

## Course connection

In the SDR course this lab fits after the first IQ-recording lab:

1. record or simulate a tone;
2. inspect spectrum;
3. detect target tone;
4. compare MATLAB / Python / C++ results;
5. discuss what should be moved to FPGA.

## FPGA / Zynq discussion

Potential mapping:

| Block | FPGA candidate? | Notes |
|---|---|---|
| Window multiply | yes | streaming-friendly |
| FFT | yes | use vendor IP core |
| Goertzel | yes | low-resource detector |
| Magnitude / power | yes | fixed-point friendly |

## Extension tasks

- add C++ FFT implementation or integrate FFTW / KissFFT;
- compare FFT bin power against Goertzel output;
- add automatic peak detection;
- process a real HDSDR recording;
- repeat the experiment with fixed-point scaling;
- benchmark FFT vs multiple Goertzel detectors.

## Engineering message

This lab introduces an important engineering decision:

```text
Do we need the whole spectrum, or only one frequency?
```

The correct answer depends on the system objective, latency budget, CPU/FPGA resources, and required observability.
