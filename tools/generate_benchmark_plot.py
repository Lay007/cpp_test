#!/usr/bin/env python3
"""Create benchmark visualization from benchmark Markdown table."""

from __future__ import annotations

import re
from pathlib import Path

import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[1]
REPORT = ROOT / "build" / "reports" / "dsp_performance.md"
OUT = ROOT / "docs" / "assets" / "benchmark_results.png"
OUT.parent.mkdir(parents=True, exist_ok=True)


def parse_report(path: Path):
    if not path.exists():
        raise FileNotFoundError(path)
    rows = []
    for line in path.read_text(encoding="utf-8").splitlines():
        if not line.startswith("|") or "---" in line or "Benchmark" in line or "Method" in line:
            continue
        cols = [c.strip() for c in line.strip("|").split("|")]
        if len(cols) < 3:
            continue
        name = cols[0]
        nums = re.findall(r"[-+]?\d*\.\d+|\d+", cols[1] + " " + cols[2])
        try:
            time_ms = float(cols[2])
        except ValueError:
            continue
        rows.append((name, time_ms))
    return rows


def main() -> None:
    rows = parse_report(REPORT)
    if not rows:
        raise RuntimeError("No benchmark rows found")
    names = [r[0] for r in rows]
    values = [r[1] for r in rows]

    plt.figure(figsize=(11, 5.5))
    y = range(len(names))
    plt.barh(list(y), values)
    plt.yticks(list(y), names)
    plt.xlabel("Average time, ms")
    plt.title("DSP benchmark results")
    plt.grid(True, axis="x", alpha=0.35)
    for i, v in enumerate(values):
        plt.text(v, i, f" {v:.3f} ms", va="center")
    plt.tight_layout()
    plt.savefig(OUT, dpi=160)
    print(f"Saved {OUT}")


if __name__ == "__main__":
    main()
