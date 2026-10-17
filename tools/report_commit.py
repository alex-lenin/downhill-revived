#!/usr/bin/env python3
"""Regenerate the progress report and commit it: README.md table, progress/report.json, objdiff.json.

For tools that promote functions (nearmiss_scan, the flag sweep, the permuter batch), so every
match shows up in the README right away. tools/report.py builds the whole image first (it must MATCH) under the src lock.

Usage: python tools/report_commit.py LABEL [FUNC ...]   (commit: "Progress report after LABEL: ...")
       --no-push   don't git push
"""
import subprocess, sys, os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def run(cmd):
    return subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)


def main():
    args = [a for a in sys.argv[1:] if a != '--no-push']
    if not args:
        sys.exit(__doc__)
    label, funcs = args[0], args[1:]
    r = run([sys.executable, 'tools/report.py'])
    if r.returncode:
        sys.exit('report failed:\n' + (r.stdout + r.stderr)[-1500:])
    run(['git', 'add', 'progress/report.json', 'objdiff.json', 'README.md'])
    what = '%d promoted (%s)' % (len(funcs), ' '.join(funcs)) if funcs else 'refresh'
    c = run(['git', 'commit', '-q', '-m', 'Progress report after %s: %s' % (label, what)])
    if c.returncode == 0 and '--no-push' not in sys.argv:
        run(['git', 'push', '-q'])
    print(r.stdout.strip().split('\n')[-1] if r.stdout.strip() else 'report done')


if __name__ == '__main__':
    main()
