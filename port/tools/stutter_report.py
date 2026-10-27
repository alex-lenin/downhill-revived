"""Summary of a stutter run's folder (stutter_race.py, or a player's logs gathered in one folder):
frame times from pacing-present.csv, how Windows showed the pictures (presentmon.csv), and the
DHD_SPIKES lines of dhd.log by kind (render-thread parts, swapchain acquire, readbacks, waits).

    python port/tools/stutter_report.py FOLDER
"""
from pathlib import Path
import collections
import csv
import re
import sys


def lows(gaps):
    s = sorted(gaps)
    n = len(s)
    if not n:
        return 'no pictures'
    mean = lambda v: sum(v) / len(v)
    return ('pictures %d; frame ms p50 %.2f p99 %.2f p99.9 %.2f max %.1f; 1%% low %.0f fps, 0.1%% low %.0f fps; '
            '>10 ms %d, >20 ms %d, >33 ms %d, >100 ms %d' % (
                n, s[n // 2], s[int(n * .99)], s[int(n * .999)], s[-1], 1000 / mean(s[int(n * .99):]),
                1000 / mean(s[int(n * .999):]), sum(g > 10 for g in s), sum(g > 20 for g in s),
                sum(g > 33 for g in s), sum(g > 100 for g in s)))


def main():
    folder = Path(sys.argv[1])
    present = folder / 'pacing-present.csv'
    if present.exists():
        p = [float(r['presented']) for r in csv.DictReader(open(present))]
        # the race only: from the first second of steady pictures after the menus is not known
        # here, so all pictures count (loading screens included)
        print('game:      ', lows([(b - a) * 1000 for a, b in zip(p, p[1:])]))
    monitor = folder / 'presentmon.csv'
    if monitor.exists():
        rows = list(csv.DictReader(open(monitor)))
        pid = folder / 'game.pid'
        if pid.exists():  # stutter_race.py records every process
            rows = [r for r in rows if r['ProcessID'] == pid.read_text().strip()]
        modes = collections.Counter(r['PresentMode'] for r in rows)
        print('presentmon: modes %s; tearing allowed %s' % (dict(modes.most_common()),
              dict(collections.Counter(r['AllowsTearing'] for r in rows))))
        num = lambda x: float(x) if x not in (None, '', 'NA') else None  # None: a cut last row
        shown = [num(r['MsBetweenDisplayChange']) for r in rows]
        print('  between screen changes:', lows([x for x in shown if x is not None]))
        until = sorted(x for x in (num(r['MsUntilDisplayed']) for r in rows) if x is not None)
        if until:
            print('  present to screen ms: p50 %.1f p99 %.1f' % (until[len(until) // 2], until[int(len(until) * .99)]))
    log = folder / 'dhd.log'
    if log.exists():
        text = log.read_text(errors='replace')
        tags = collections.defaultdict(list)
        for ms, tag in re.findall(r'ms=([\d.]+) tag=(\w+)', text):
            tags[tag].append(float(ms))
        for kind in ('spike_render', 'spike_lock', 'spike_queue', 'spike_present'):
            values = [float(v) for v in re.findall(kind + r': .*? ms=([\d.]+)', text)] if kind != 'spike_present' else \
                [float(a) + float(b) for a, b in re.findall(r'acquire_ms=([\d.]+) pace_present_ms=([\d.]+)', text)]
            tags[kind] = values
        for name, v in sorted(tags.items()):
            if v:
                print('  %-14s n %5d sum %7.0f ms max %6.1f >16 ms %d' % (name, len(v), sum(v), max(v), sum(x > 16 for x in v)))


if __name__ == '__main__':
    main()
