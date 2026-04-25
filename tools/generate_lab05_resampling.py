#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs" / "assets" / "lab05_resampling_polyphase.png"
OUT.parent.mkdir(parents=True, exist_ok=True)

fs = 8000
N = 1024
t = np.arange(N)/fs

# input signal
x = np.sin(2*np.pi*700*t) + 0.5*np.sin(2*np.pi*1800*t)

L = 3
M = 2

# naive upsample
xu = np.zeros(len(x)*L)
xu[::L] = x

# simple FIR (low-pass)
num_taps = 63
n = np.arange(num_taps) - (num_taps-1)/2
h = np.sinc(n/L) * np.hanning(num_taps)
h /= np.sum(h)

xf = np.convolve(xu, h, mode='same')

# decimate
y = xf[::M]

plt.figure(figsize=(10,6))

plt.subplot(3,1,1)
plt.plot(x[:200])
plt.title("Input signal")

plt.subplot(3,1,2)
plt.plot(xu[:400])
plt.title("Upsampled (zero insertion)")

plt.subplot(3,1,3)
plt.plot(y[:200])
plt.title("Resampled output (L/M)")

plt.tight_layout()
plt.savefig(OUT, dpi=150)
print("Saved:", OUT)
