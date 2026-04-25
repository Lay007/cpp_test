# Course Bridge

`cpp-dsp-showcase` can act as the C++ implementation track for the companion SDR course:

- course repository: <https://github.com/Lay007/zynq-sdr-course>
- C++ DSP reference: <https://github.com/Lay007/cpp-dsp-showcase>

## Role in the learning flow

The course explains the theory and SDR context.
This repository provides the engineering layer where students can:

1. inspect DSP kernels in readable C++
2. validate behavior with deterministic tests
3. generate plots and benchmark reports
4. compare desktop implementations with future FPGA candidates

## Suggested progression

| Stage | Focus | Activity in this repository |
|---|---|---|
| 1 | Build environment | configure, build, run tests |
| 2 | Signals and spectra | generate tones, inspect RMS and response |
| 3 | FIR filtering | design and validate a low-pass filter |
| 4 | Tone detection | analyze single-tone detection with Goertzel |
| 5 | Delay estimation | study GCC-PHAT behavior and limits |
| 6 | Resampling | inspect `L/M` sample-rate conversion |
| 7 | SDR recordings | process IQ captures with the CI16 demo |
| 8 | Hardware migration | compare software baseline against hardware-oriented implementations |

## Core message

The value of the repository is not only that the algorithms run.
The value is that each algorithm has a visible engineering path:

```text
theory -> reference implementation -> tests -> benchmark -> hardware candidate
```

That makes the project a practical bridge between DSP education and real implementation work.
