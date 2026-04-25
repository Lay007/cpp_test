#!/usr/bin/env python3
"""Generate visual DSP validation artifacts for cpp-dsp-showcase.

The script intentionally uses only NumPy and Matplotlib so it can be run locally
or from CI without requiring a heavy DSP stack.
"""

from __future__ import annotations

import math
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

ROOT = Path(__file__).resolve().parents[1]
ASSETS = ROOT / "docs" / "assets"
ASSETS.mkdir(parents=True, exist_ok=True)


def design_lowpass(num_taps: int = 127, cutoff: float = 0.12) -> np.ndarray:
    n = np.arange(num_taps) - (num_taps - 1) / 2
    h = 2 * cutoff * np.sinc(2 * cutoff * n)
    h *= np.blackman(num_taps)
    h /= np.sum(h)
    return h


def save_fir_response() -> None:
    h = design_lowpass()
    nfft = 8192
    h_fft = np.fft.rfft(h, nfft)
    freq = np.fft.rfftfreq(nfft, d=1.0)
    mag_db = 20 * np.log10(np.maximum(np.abs(h_fft), 1e-12))

    plt.figure(figsize=(10, 4.8))
    plt.plot(freq, mag_db, linewidth=2)
    plt.axvline(0.12, linestyle="--", linewidth=1)
    plt.title("Windowed-sinc FIR low-pass response")
    plt.xlabel("Normalized frequency [cycles/sample]")
    plt.ylabel("Magnitude [dB]")
    plt.grid(True, alpha=0.35)
    plt.tight_layout()
    plt.savefig(ASSETS / "fir_response.png", dpi=180)
    plt.close()


def goertzel_power(x: np.ndarray, fs: float, freq: float) -> float:
    n = len(x)
    k = int(round(n * freq / fs))
    omega = 2.0 * math.pi * k / n
    coeff = 2.0 * math.cos(omega)
    s_prev = 0.0
    s_prev2 = 0.0
    for sample in x:
        s = sample + coeff * s_prev - s_prev2
        s_prev2 = s_prev
        s_prev = s
    return s_prev2**2 + s_prev**2 - coeff * s_prev * s_prev2


def save_goertzel_detection() -> None:
    fs = 8000.0
    n = 1024
    t = np.arange(n) / fs
    target = 1000.0
    x = 0.9 * np.sin(2 * np.pi * target * t) + 0.15 * np.sin(2 * np.pi * 2200 * t)
    x += 0.05 * np.random.default_rng(7).normal(size=n)

    freqs = np.arange(200, 3001, 50)
    powers = np.array([goertzel_power(x, fs, f) for f in freqs])
    powers_db = 10 * np.log10(np.maximum(powers / np.max(powers), 1e-12))

    plt.figure(figsize=(10, 4.8))
    plt.plot(freqs, powers_db, marker="o", markersize=3, linewidth=1.8)
    plt.axvline(target, linestyle="--", linewidth=1)
    plt.title("Goertzel tone detection")
    plt.xlabel("Test frequency [Hz]")
    plt.ylabel("Normalized power [dB]")
    plt.grid(True, alpha=0.35)
    plt.tight_layout()
    plt.savefig(ASSETS / "goertzel_detection.png", dpi=180)
    plt.close()


def save_gcc_phat_delay() -> None:
    rng = np.random.default_rng(11)
    n = 512
    true_delay = 37
    source = rng.normal(size=n)
    source *= np.hanning(n)
    delayed = np.concatenate([np.zeros(true_delay), source[:-true_delay]])
    delayed += 0.03 * rng.normal(size=n)

    nfft = 2 * n
    x = np.fft.fft(source, nfft)
    y = np.fft.fft(delayed, nfft)
    cross = x * np.conj(y)
    phat = cross / np.maximum(np.abs(cross), 1e-12)
    corr = np.fft.ifft(phat).real
    corr = np.fft.fftshift(corr)
    lags = np.arange(-nfft // 2, nfft // 2)
    estimated_delay = lags[np.argmax(corr)]

    plt.figure(figsize=(10, 4.8))
    plt.plot(lags, corr, linewidth=1.8)
    plt.axvline(estimated_delay, linestyle="--", linewidth=1)
    plt.title(f"GCC-PHAT delay estimate: {estimated_delay} samples")
    plt.xlabel("Lag [samples]")
    plt.ylabel("PHAT correlation")
    plt.xlim(-120, 120)
    plt.grid(True, alpha=0.35)
    plt.tight_layout()
    plt.savefig(ASSETS / "gcc_phat_delay.png", dpi=180)
    plt.close()


def main() -> None:
    save_fir_response()
    save_goertzel_detection()
    save_gcc_phat_delay()
    print(f"Generated DSP plots in {ASSETS}")


if __name__ == "__main__":
    main()
