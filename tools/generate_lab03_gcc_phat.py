#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs" / "assets" / "lab03_gcc_phat_delay.png"
OUT.parent.mkdir(parents=True, exist_ok=True)

rng = np.random.default_rng(42)
N = 512
true_delay = 37

x = rng.normal(size=N)
x *= np.hanning(N)

y = np.concatenate([np.zeros(true_delay), x[:-true_delay]])
y += 0.03*rng.normal(size=N)

nfft = 2*N
X = np.fft.fft(x, nfft)
Y = np.fft.fft(y, nfft)

R = X * np.conj(Y)
PHAT = R / np.maximum(np.abs(R), 1e-12)

corr = np.fft.ifft(PHAT).real
corr = np.fft.fftshift(corr)

lags = np.arange(-nfft//2, nfft//2)
est_delay = lags[np.argmax(corr)]

plt.figure(figsize=(10,4))
plt.plot(lags, corr)
plt.axvline(est_delay, linestyle='--')
plt.title(f"GCC-PHAT delay estimate: {est_delay} samples")
plt.xlabel("Lag")
plt.ylabel("Correlation")
plt.grid(True)
plt.xlim(-100,100)
plt.tight_layout()
plt.savefig(OUT, dpi=150)

print("Saved:", OUT)
