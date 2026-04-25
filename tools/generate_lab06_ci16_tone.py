#!/usr/bin/env python3
import numpy as np
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "data" / "lab06_simulated_ci16.iq"
OUT.parent.mkdir(parents=True, exist_ok=True)

fs = 1024000
N = 50000

t = np.arange(N) / fs

# tone at 100 kHz
f0 = 100000

i = 0.8*np.cos(2*np.pi*f0*t)
q = 0.8*np.sin(2*np.pi*f0*t)

# scale to int16
i16 = np.int16(i * 32767)
q16 = np.int16(q * 32767)

interleaved = np.empty(2*N, dtype=np.int16)
interleaved[0::2] = i16
interleaved[1::2] = q16

interleaved.tofile(OUT)

print("Generated:", OUT)
