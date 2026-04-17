# Local Performance Snapshot

Russian version: [perf_report_ru.md](perf_report_ru.md)

Date: 2026-04-17  
Platform: Windows (MSVC, Release)

Source benchmark report was generated via:

```bash
cmake --build build --config Release --target benchmark_report
```

Results:

| Benchmark | Samples | Avg time (ms) | Throughput (MSa/s) |
|---|---:|---:|---:|
| FIR 127-tap convolution | 32768 | 2.794 | 11.727 |
| Goertzel power detector | 8192 | 0.044 | 184.380 |
| GCC-PHAT delay estimate | 1024 | 179.096 | 0.006 |

Notes:
- These numbers are hardware/compiler/config dependent.
- GCC-PHAT currently uses a naive DFT implementation and is intentionally left as an optimization target.
