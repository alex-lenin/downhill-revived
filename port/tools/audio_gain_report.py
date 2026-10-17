"""Summarize deterministic DHD_SND_GAIN_TRACE bins (each contains 4800 frames).

Component energies are measured before the final master gain. They include
stereo channel correlations within each component, but RMS values from separate
components must not be added to estimate the final mix.
"""
import argparse
import csv
import json
import math
from pathlib import Path


def summarize(path, start=0.0, duration=None):
    with Path(path).open(newline="", encoding="utf-8") as file:
        rows = [{key: int(value) for key, value in row.items()}
                for row in csv.DictReader(file)]
    end = float("inf") if duration is None else start + duration
    selected = [row for row in rows if start <= row["sample"] / 48000 < end]
    if not selected:
        raise ValueError("no complete 100-ms bins in the requested interval")
    result = dict(start_seconds=selected[0]["sample"] / 48000,
                  duration_seconds=len(selected) / 10,
                  measurement="pre-master component sums; RMS values do not add")
    for component in ("sfx", "stream", "wet"):
        total = sum(row[f"{component}_energy_{channel}"]
                    for row in selected for channel in ("left", "right"))
        result[component] = dict(
            stereo_rms=math.sqrt(total / (len(selected) * 4800 * 2)),
            peak=max(row[f"{component}_peak"] for row in selected))
    for field in ("max_voice_gain", "min_lfo_volume", "max_lfo_volume",
                  "min_duck_gain", "max_duck_gain", "master_gain"):
        values = [row[field] for row in selected]
        # A zero maximum denotes no ducker update in that interval, not mute.
        if "duck" in field:
            values = [row[field] for row in selected if row["max_duck_gain"]]
        result[field] = dict(min=min(values), max=max(values)) if values else None
    result["silent_source_bins"] = sum(
        row["voices"] == 0 and row["streams"] == 0 for row in selected)
    result["max_voices"] = max(row["voices"] for row in selected)
    result["max_streams"] = max(row["streams"] for row in selected)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("trace", type=Path)
    parser.add_argument("--start", type=float, default=0)
    parser.add_argument("--duration", type=float)
    parser.add_argument("--json", type=Path)
    args = parser.parse_args()
    report = summarize(args.trace, args.start, args.duration)
    text = json.dumps(report, indent=2) + "\n"
    if args.json:
        args.json.write_text(text, encoding="utf-8")
    print(text, end="")


if __name__ == "__main__":
    main()
