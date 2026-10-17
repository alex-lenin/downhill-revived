"""List src/ files whose inline asm breaks today's source rules (tools/source_rules.py warnings).
These matched before the rules were tightened; they still need clean C.
usage: python port/tools/rules_audit.py"""
import glob, os, sys
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import source_rules  # noqa: E402

bad = {}
for p in sorted(glob.glob(os.path.join(ROOT, 'src', '**', '*.c*'), recursive=True)):
    w = source_rules.warnings(open(p, encoding='utf-8', errors='replace').read())
    if w:
        bad[os.path.relpath(p, ROOT).replace(os.sep, '/')] = w
for f, w in bad.items():
    print('%s: %s' % (f, '; '.join(x[:70] for x in w[:4])))
print('%d files break the current rules' % len(bad))
