#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs" / "assets" / "lab04_fir_time_vs_fft.png"
OUT.parent.mkdir(parents=True, exist_ok=True)

N = np.array([1e3, 2e3, 5e3, 1e4])
M = np.array([31, 127, 512, 2048])

# Estimate operations
# Direct FIR ~ N*M
# FFT ~ N log2(N)

def direct_cost(n, m):
    return n*m

def fft_cost(n):
    return n*np.log2(n)

plt.figure(figsize=(8,5))

for m in M:
    plt.plot(N, direct_cost(N, m), label=f"Direct FIR M={int(m)}")

plt.plot(N, fft_cost(N)*10, '--', label="FFT (scaled)")

plt.xscale('log')
plt.yscale('log')
plt.xlabel("Signal length N")
plt.ylabel("Relative cost")
plt.title("FIR: Time-domain vs FFT complexity")
plt.grid(True, which='both')
plt.legend()

plt.tight_layout()
plt.savefig(OUT, dpi=150)
print("Saved:", OUT)
