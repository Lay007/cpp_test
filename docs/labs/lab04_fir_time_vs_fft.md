# Lab 04 — FIR filtering: time-domain vs FFT-based convolution

## Purpose

This lab explains a classic DSP engineering decision:

```text
When should FIR filtering be implemented directly in time domain,
and when should it be replaced by FFT-based convolution?
```

The answer depends on signal length, filter length, latency, memory budget, implementation complexity, and target platform.

## Learning outcomes

After this lab, a student should be able to:

- explain direct FIR convolution;
- estimate computational complexity;
- understand why FFT-based convolution can be faster for long filters;
- understand overlap-save / overlap-add as block-processing methods;
- connect FIR optimization to CPU SIMD and FPGA acceleration;
- interpret benchmark results as engineering evidence.

## Signal-processing background

A direct FIR filter computes:

```text
y[n] = sum h[k] * x[n-k]
```

For `N` input samples and `M` filter taps, the direct implementation costs approximately:

```text
O(N · M)
```

FFT-based convolution transforms convolution into multiplication in the frequency domain:

```text
conv(x, h) = IFFT(FFT(x) · FFT(h))
```

For large blocks, this can approach:

```text
O(N log N)
```

with additional overhead from block handling and transforms.

## Practical comparison

| Method | Best for | Complexity | Strength | Weakness |
|---|---|---:|---|---|
| Direct FIR | short filters, low latency | O(N·M) | simple, streaming-friendly | expensive for long filters |
| SIMD direct FIR | short / medium filters | O(N·M) | high throughput | platform-specific |
| FFT convolution | long filters, large blocks | O(N log N) | scalable | block latency + complexity |
| Overlap-save | continuous streams | O(N log N) | efficient streaming blocks | needs buffering |
| FPGA FIR | fixed structure / real-time | depends | deterministic latency | resource planning required |

## Experiment idea

Compare estimated cost for different filter lengths:

| Filter taps | Direct FIR | FFT-based candidate |
|---:|---|---|
| 31 | usually excellent | overhead may dominate |
| 127 | good baseline | depends on block size |
| 1024 | expensive | FFT likely attractive |
| 4096 | very expensive | FFT strongly attractive |

## Suggested command

Generate the complexity comparison plot:

```bash
pip install numpy matplotlib
python tools/generate_lab04_fir_complexity.py
```

Output:

```text
docs/assets/lab04_fir_time_vs_fft.png
```

## Benchmark connection

The repository already has a benchmark workflow and reports. Lab 04 should be used to discuss:

1. current direct FIR benchmark;
2. expected scaling with filter length;
3. where FFT acceleration becomes useful;
4. how SIMD changes the break-even point.

## Course connection

This lab fits after spectrum and delay-estimation labs because students already understand:

- frequency response;
- spectra;
- transform-domain thinking;
- benchmark interpretation.

## CPU optimization direction

Recommended CPU path:

1. direct scalar FIR baseline;
2. memory layout cleanup;
3. SIMD vectorization;
4. multi-thread block processing;
5. FFT convolution for long filters.

## FPGA / Zynq discussion

Potential hardware mapping:

| Block | FPGA candidate? | Notes |
|---|---|---|
| Short FIR | yes | natural DSP-slice mapping |
| Long FIR | yes | may need polyphase / FFT strategy |
| FFT convolution | yes | vendor FFT IP + complex multiply |
| Overlap-save controller | yes | buffering and state machine |

## Extension tasks

- implement a simple FFT-based convolution prototype;
- add overlap-save processing;
- benchmark direct FIR vs FFT convolution;
- test different filter lengths;
- add AVX2/IPP backend;
- compare CPU and FPGA resource trade-offs;
- discuss latency vs throughput.

## Engineering message

The main design lesson is:

```text
The fastest algorithm is context-dependent.
```

For short filters, simple direct FIR can be the best engineering choice. For long filters and large blocks, FFT-based convolution becomes a strong candidate. A professional DSP implementation should keep both paths visible and benchmarked.
