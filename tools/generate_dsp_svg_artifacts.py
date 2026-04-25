#!/usr/bin/env python3
"""Generate IEEE-like SVG DSP artifacts from computed data."""
from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs" / "assets"
OUT.mkdir(parents=True, exist_ok=True)

plt.rcParams.update({
    "font.family": "DejaVu Sans",
    "axes.edgecolor": "black",
    "axes.linewidth": 1.0,
    "grid.color": "0.80",
    "grid.linestyle": "--",
    "grid.linewidth": 0.7,
})

MAIN = "#2563eb"
MARK = "#dc2626"


def save_fig(path):
    plt.tight_layout()
    plt.savefig(path, format="svg", bbox_inches="tight")
    plt.close()


def fir():
    taps = 127
    cutoff = 0.12
    n = np.arange(taps) - (taps - 1) / 2
    h = 2 * cutoff * np.sinc(2 * cutoff * n)
    h *= np.blackman(taps)
    h /= h.sum()
    nfft = 8192
    H = np.fft.rfft(h, nfft)
    f = np.fft.rfftfreq(nfft, d=1.0)
    mag = 20*np.log10(np.maximum(np.abs(H), 1e-8))
    plt.figure(figsize=(9.8, 4.3))
    plt.plot(f, mag, color=MAIN, linewidth=2.2, label="FIR response")
    plt.axvline(cutoff, color=MARK, linestyle="--", linewidth=1.6, label="cutoff")
    plt.title("FIR low-pass frequency response", fontweight="bold")
    plt.xlabel("Normalized frequency, cycles/sample")
    plt.ylabel("Magnitude, dB")
    plt.xlim(0, 0.5)
    plt.ylim(-90, 5)
    plt.xticks(np.arange(0, 0.51, 0.1))
    plt.yticks([-80, -60, -40, -20, 0])
    plt.grid(True)
    plt.legend(loc="upper right", frameon=True)
    save_fig(OUT / "fir_response.svg")


def goertzel_power(x, fs, freq):
    n = len(x)
    k = int(round(n * freq / fs))
    omega = 2*np.pi*k/n
    coeff = 2*np.cos(omega)
    s1 = 0.0
    s2 = 0.0
    for sample in x:
        s0 = sample + coeff*s1 - s2
        s2 = s1
        s1 = s0
    return s2*s2 + s1*s1 - coeff*s1*s2


def goertzel():
    rng = np.random.default_rng(7)
    fs = 8000.0
    N = 1024
    t = np.arange(N)/fs
    target = 1000.0
    x = np.sin(2*np.pi*target*t) + 0.25*np.sin(2*np.pi*2300*t) + 0.05*rng.normal(size=N)
    freqs = np.arange(0, 4001, 50)
    p = np.array([goertzel_power(x, fs, f if f > 0 else 1.0) for f in freqs])
    p /= p.max()
    plt.figure(figsize=(9.8, 4.3))
    plt.plot(freqs, p, color=MAIN, linewidth=2.2, label="Goertzel response")
    plt.axvline(target, color=MARK, linestyle="--", linewidth=1.6, label="target tone")
    plt.title("Goertzel tone detection", fontweight="bold")
    plt.xlabel("Frequency, Hz")
    plt.ylabel("Normalized power")
    plt.xlim(0, 4000)
    plt.ylim(0, 1.05)
    plt.xticks(np.arange(0, 4001, 500))
    plt.yticks(np.arange(0, 1.01, 0.2))
    plt.grid(True)
    plt.legend(loc="upper right", frameon=True)
    save_fig(OUT / "goertzel_detection.svg")


def gcc():
    rng = np.random.default_rng(42)
    N = 512
    delay = 37
    x = rng.normal(size=N) * np.hanning(N)
    y = np.concatenate([np.zeros(delay), x[:-delay]]) + 0.03*rng.normal(size=N)
    nfft = 2*N
    X = np.fft.fft(x, nfft)
    Y = np.fft.fft(y, nfft)
    R = X*np.conj(Y)
    G = R/np.maximum(np.abs(R), 1e-12)
    c = np.fft.fftshift(np.fft.ifft(G).real)
    lags = np.arange(-nfft//2, nfft//2)
    c = (c - c.min()) / max(c.max() - c.min(), 1e-12)
    est = lags[np.argmax(c)]
    plt.figure(figsize=(9.8, 4.3))
    plt.plot(lags, c, color=MAIN, linewidth=2.2, label="GCC-PHAT")
    plt.axvline(est, color=MARK, linestyle="--", linewidth=1.6, label=f"delay = {est} samples")
    plt.title("GCC-PHAT delay estimation", fontweight="bold")
    plt.xlabel("Lag, samples")
    plt.ylabel("Normalized correlation")
    plt.xlim(-120, 120)
    plt.ylim(0, 1.05)
    plt.xticks(np.arange(-120, 121, 40))
    plt.yticks(np.arange(0, 1.01, 0.2))
    plt.grid(True)
    plt.legend(loc="upper right", frameon=True)
    save_fig(OUT / "gcc_phat_delay.svg")


def main():
    fir(); goertzel(); gcc()
    print("Generated SVG DSP artifacts in", OUT)

if __name__ == "__main__":
    main()
