#!/usr/bin/env python3
"""Create benchmark visualizations from benchmark Markdown table.

Outputs:
- docs/assets/benchmark_results.png
- docs/assets/benchmark_dashboard.svg
"""

from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[1]
REPORT = ROOT / "build" / "reports" / "dsp_performance.md"
OUT_PNG = ROOT / "docs" / "assets" / "benchmark_results.png"
OUT_SVG = ROOT / "docs" / "assets" / "benchmark_dashboard.svg"
OUT_PNG.parent.mkdir(parents=True, exist_ok=True)


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
            if idx < len(cols) and ("avg time" in col_name or "time" in col_name):
                time_ms = _to_float(cols[idx])
                break
        if time_ms is not None:
            rows.append((name, time_ms))
    return rows


def write_png(rows):
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
    plt.savefig(OUT_PNG, dpi=160)
    plt.close()


def write_svg(rows):
    values = [r[1] for r in rows]
    max_ms = max(values) if values else 1.0
    if max_ms <= 0:
        max_ms = 1.0

    plot_x = 155
    plot_y = 145
    plot_w = 800
    plot_h = 270
    base_y = plot_y + plot_h
    scale = plot_h / max_ms
    bar_w = 52
    gap = 44

    svg = []
    svg.append('<svg xmlns="http://www.w3.org/2000/svg" width="1200" height="590" viewBox="0 0 1200 590" role="img" aria-labelledby="title desc">')
    svg.append('<title id="title">DSP benchmark dashboard</title>')
    svg.append('<desc id="desc">Benchmark dashboard generated from real benchmark report.</desc>')
    svg.append('<style>.bg{fill:#f8fafc}.title{font:700 32px Arial;fill:#0f172a}.sub{font:400 16px Arial;fill:#475569}.card{fill:#fff;stroke:#cbd5e1;stroke-width:2;rx:18}.axis{stroke:#334155;stroke-width:2}.grid{stroke:#e2e8f0;stroke-width:1}.bar{fill:#2563eb}.txt{font:400 12px Arial;fill:#334155}.label{font:700 12px Arial;fill:#0f172a}.val{font:700 12px Arial;fill:#2563eb}.small{font:400 12px Arial;fill:#64748b}.legend{fill:#fff;stroke:#cbd5e1;stroke-width:1.6;rx:14}</style>')
    svg.append('<rect class="bg" width="1200" height="590"/>')
    svg.append('<text x="60" y="60" class="title">DSP Benchmark Dashboard</text>')
    svg.append('<text x="60" y="88" class="sub">Generated from actual benchmark report: build/reports/dsp_performance.md</text>')
    svg.append('<rect x="60" y="120" width="1080" height="400" class="card"/>')
    svg.append(f'<line x1="{plot_x}" y1="{base_y}" x2="{plot_x+plot_w}" y2="{base_y}" class="axis"/>')
    svg.append(f'<line x1="{plot_x}" y1="{plot_y}" x2="{plot_x}" y2="{base_y}" class="axis"/>')

    for frac in [0.25, 0.5, 0.75, 1.0]:
        y = base_y - plot_h * frac
        v = max_ms * frac
        svg.append(f'<line x1="{plot_x}" y1="{y:.1f}" x2="{plot_x+plot_w}" y2="{y:.1f}" class="grid"/>')
        svg.append(f'<text x="88" y="{y+4:.1f}" class="txt">{v:.2f} ms</text>')

    x = plot_x + 40
    for name, ms in rows:
        h = max(2.0, ms * scale)
        y = base_y - h
        svg.append(f'<rect x="{x}" y="{y:.1f}" width="{bar_w}" height="{h:.1f}" class="bar"/>')
        svg.append(f'<text x="{x-2}" y="{y-8:.1f}" class="val">{ms:.3f} ms</text>')
        short = name.replace("Goertzel ", "Gtz ").replace("FIR ", "FIR ").replace("GCC-PHAT ", "GCC ")
        svg.append(f'<text x="{x-18}" y="{base_y+24}" class="label">{short}</text>')
        x += bar_w + gap

    svg.append('<rect x="865" y="155" width="230" height="92" class="legend"/>')
    svg.append('<text x="890" y="188" class="label">Real benchmark data</text>')
    svg.append('<rect x="890" y="205" width="18" height="18" class="bar"/>')
    svg.append('<text x="920" y="219" class="txt">Average runtime, ms</text>')
    svg.append('<text x="60" y="552" class="small">Lower is better. Values depend on CPU, compiler, build type, input sizes and CI runner load.</text>')
    svg.append('</svg>')
    OUT_SVG.write_text("\n".join(svg), encoding="utf-8")


def main() -> None:
    rows = parse_report(REPORT)
    if not rows:
        print(f"No benchmark rows found in {REPORT}; writing placeholder plot.")
        rows = [("No benchmark data", 0.0)]
    write_png(rows)
    write_svg(rows)
    print(f"Saved {OUT_PNG}")
    print(f"Saved {OUT_SVG}")


if __name__ == "__main__":
    main()
