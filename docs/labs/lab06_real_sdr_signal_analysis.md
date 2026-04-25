# Lab 06 — Real SDR signal analysis

## Purpose

This lab closes the loop between the SDR course and the C++ DSP backend.

The goal is to process a real or simulated SDR recording with the same engineering mindset used in production DSP systems:

```text
RF signal
   ↓
rtl-sdr / HDSDR / GNU Radio
   ↓
raw IQ recording
   ↓
C++ DSP analysis
   ↓
plots, metrics, validation
   ↓
future FPGA / Zynq acceleration
```

## Learning outcomes

After this lab, a student should be able to:

- record or prepare an IQ file;
- understand interleaved complex sample formats;
- analyze amplitude level and RMS;
- detect a tone or narrowband component;
- compare C++ output with Python / MATLAB / GNU Radio;
- identify what parts of the pipeline are suitable for FPGA implementation.

## Supported input format

The first practical format is interleaved signed 16-bit complex IQ:

```text
I0, Q0, I1, Q1, I2, Q2, ...
```

Short name:

```text
ci16
```

Recommended file path:

```text
data/lab06_real_or_simulated_ci16.iq
```

## Recording options

### Option A — HDSDR

1. Configure the SDR receiver.
2. Select an appropriate sample rate.
3. Record raw IQ data.
4. Save/export as complex int16 if possible.
5. Run the C++ analyzer.

### Option B — rtl_sdr command-line tools

Depending on the toolchain, rtl-sdr commonly produces unsigned 8-bit IQ. For this course track, convert or generate a ci16 file before using the current C++ analyzer.

### Option C — simulated recording

Use the helper script:

```bash
python tools/generate_lab06_ci16_tone.py
```

It produces a deterministic ci16 IQ file for testing the pipeline without hardware.

## Suggested parameters

| Parameter | Recommended value |
|---|---:|
| File format | ci16 IQ |
| Sample rate | 1.024 MSa/s for first SDR experiments |
| Duration | 0.1–2 seconds |
| Target tone | known offset frequency |
| Analysis | RMS + Goertzel + optional FFT |

## Example workflow

```bash
pip install numpy
python tools/generate_lab06_ci16_tone.py

cmake -S . -B build -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/dsp_core/iq_analysis_demo data/lab06_simulated_ci16.iq 1024000 100000
```

Expected output includes:

```text
Samples: ...
RMS: ...
Goertzel power @ 100000 Hz: ...
```

## Validation checklist

| Step | Check |
|---|---|
| File generation/recording | file size is non-zero |
| IQ interpretation | sample count is plausible |
| Scaling | normalized values are within expected range |
| RMS | non-zero and not clipped |
| Tone detection | Goertzel power increases near target |
| Reproducibility | same input gives same report |

## Engineering discussion

### Why start with files?

File-based processing is easier to test, debug, and reproduce than live streaming. Once the DSP blocks are validated, the same logic can be moved to real-time input.

### Why ci16?

Complex int16 is common in RF pipelines because it is compact, efficient, and close to what ADC-oriented systems produce after digital downconversion.

### What is missing for production?

- metadata handling;
- robust file format detection;
- overload/clipping metrics;
- streaming input;
- FFT spectrum report;
- multi-channel synchronization;
- fixed-point test vectors.

## FPGA / Zynq connection

Potential mapping:

| Stage | FPGA candidate? | Comment |
|---|---|---|
| IQ scaling | yes | fixed-point normalization |
| FIR filtering | yes | DSP slices |
| Goertzel | yes | narrowband detector |
| FFT spectrum | yes | vendor FFT IP |
| GCC-PHAT | yes | FFT + complex multiply + peak search |
| Resampler | yes | polyphase FIR |

## Course message

This lab demonstrates the full educational and engineering chain:

```text
real signal → stored IQ → C++ reference → validation → optimization → FPGA candidate
```

That is the bridge between theory, software DSP, SDR practice, and hardware implementation.
