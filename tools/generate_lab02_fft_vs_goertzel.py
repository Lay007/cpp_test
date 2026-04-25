#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs" / "assets" / "lab02_fft_vs_goertzel.png"
OUT.parent.mkdir(parents=True, exist_ok=True)

fs = 8000
N = 1024
t = np.arange(N) / fs

# signal
x = 1.0*np.sin(2*np.pi*1000*t)
x += 0.25*np.sin(2*np.pi*2300*t)
x += 0.05*np.random.randn(N)

# window
w = np.hanning(N)
xw = x*w

# FFT
X = np.fft.rfft(xw)
f = np.fft.rfftfreq(N, 1/fs)
mag = 20*np.log10(np.maximum(np.abs(X), 1e-12))

# Goertzel

def goertzel(x, fs, freq):
    n = len(x)
    k = int(0.5 + n*freq/fs)
    w = 2*np.pi*k/n
    c = 2*np.cos(w)
    s0 = s1 = s2 = 0
    for v in x:
        s0 = v + c*s1 - s2
        s2 = s1
        s1 = s0
    return s1*s1 + s2*s2 - c*s1*s2

freqs = np.arange(200,3000,50)
powers = [goertzel(x, fs, f0) for f0 in freqs]
powers = 10*np.log10(np.maximum(powers/np.max(powers),1e-12))

plt.figure(figsize=(10,6))
plt.subplot(2,1,1)
plt.plot(f, mag)
plt.title("FFT Spectrum")
plt.grid(True)

plt.subplot(2,1,2)
plt.plot(freqs, powers, marker='o')
plt.title("Goertzel Response")
plt.grid(True)

plt.tight_layout()
plt.savefig(OUT, dpi=150)
print("Saved:", OUT)
