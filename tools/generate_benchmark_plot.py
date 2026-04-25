#!/usr/bin/env python3
"""Create benchmark visualization from benchmark Markdown table.

Supports both report formats used by the project:
- | Benchmark | Samples | Avg time (ms) | Throughput |
- | Method | Time (ms) | Speedup |
"""

from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[1]
REPORT = ROOT / "build" / "reports" / "dsp_performance.md"
OUT = ROOT / "docs" / "assets" / "benchmark_results.png"
OUT.parent.mkdir(parents=True, exist_ok=True)


def _to_float(text: str):
    cleaned = text.strip().replace("x", "").replace("ms", "")
    try:
        return float(cleaned)
    except ValueError:
        return None


def parse_report(path: Path):
    if not path.exists():
        raise FileNotFoundError(path)

    rows = []
    header = []

    for line in path.read_text(encoding="utf-8").splitlines():
        if not line.startswith("|"):
            continue
        cols = [c.strip() for c in line.strip().strip("|").split("|")]
        if not cols or all(set(c) <= {"-", ":", " "} for c in cols):
            continue

        lower = [c.lower() for c in cols]
        if "benchmark" in lower[0] or "method" in lower[0]:
            header = lower
            continue

        if not header:
            continue

        name = cols[0]
        time_ms = None

        for idx, col_name in enumerate(header):
            if idx >= len(cols):
                continue
            if "avg time" in col_name or "time" in col_name:
                time_ms = _to_float(cols[idx])
                break

        if time_ms is not None:
            rows.append((name, time_ms))

    return rows


def main() -> None:
    rows = parse_report(REPORT)
    if not rows:
        print(f"No benchmark rows found in {REPORT}; writing placeholder plot.")
        rows = [("No benchmark data", 0.0)]

    names = [r[0] for r in rows]
    values = [r[1] for r in rows]

    plt.figure(figsize=(11, 5.5))
    y = list(range(len(names)))
    plt.barh(y, values)
    plt.yticks(y, names)
    plt.xlabel("Average time, ms")
    plt.title("DSP benchmark results")
    plt.grid(True, axis="x", alpha=0.35)

    xmax = max(values) if values else 1.0
    if xmax <= 0:
        xmax = 1.0
    plt.xlim(0, xmax * 1.25)

    for i, v in enumerate(values):
        plt.text(v + xmax * 0.02, i, f"{v:.3f} ms", va="center")

    plt.tight_layout()
    plt.savefig(OUT, dpi=160)
    print(f"Saved {OUT}")


if __name__ == "__main__":
    main()
