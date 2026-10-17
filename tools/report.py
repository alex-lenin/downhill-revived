#!/usr/bin/env python3
"""Progress report for decomp.dev: write objdiff.json and run `objdiff-cli report generate`.

Every .text piece of the build is an objdiff unit:
  C/C++ unit  target = its retail asm assembled (build/expected/...), base = our compiled object
  asm run     target only (no base yet), so its functions count as not decompiled
Units are categorised by region (crt0, game, libgcc, engine, sdk); game/engine functions written in
assembly (splat's marker or config/handwritten.txt) and dead-strip leftovers are their own asm units,
categories 'handwritten' and 'remnant'. The README table adds a 'target' row: game + engine. C++ mangled names in base
objects are mapped to splat's plain names. Only build outputs and names go into the report —
no game bytes — so progress/report.json can be committed.

Usage: python tools/report.py        (builds first; writes objdiff.json and progress/report.json)
"""
import json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from configure import REGIONS, handwritten, remnants

OBJDIFF = os.path.join(ROOT, 'tools', 'compilers', 'objdiff', 'objdiff-cli-windows-x86_64.exe')
NM = os.path.join(ROOT, 'tools', 'compilers', 'ps2dev-binutils', 'mips64r5900el-ps2-elf-nm.exe')
FUNC = re.compile(r'func_([0-9A-Fa-f]{8})')


def region(addr):
    name = REGIONS[0][1]
    for a, n in REGIONS:
        if addr >= a:
            name = n
    return name


def run(cmd):
    p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    if p.returncode:
        sys.exit((p.stdout + p.stderr)[-3000:])
    return p.stdout


def mangled_mappings(base):
    """target (splat) name -> base (mangled C++) name, for func_XXXXXXXX symbols."""
    out = {}
    for line in run([NM, '-P', '--defined-only', base]).split('\n'):
        f = line.split()
        if len(f) < 2:
            continue
        m = FUNC.search(f[0])
        if m and f[0] != 'func_' + m.group(1):
            out['func_' + m.group(1)] = f[0]
    return out


def main():
    run([sys.executable, 'tools/configure.py'])
    run([sys.executable, '-m', 'ninja'])            # the image must MATCH first
    run([sys.executable, '-m', 'ninja', 'expected'])
    info = json.load(open(os.path.join(ROOT, 'build', 'units.json')))
    c_units = {o: (s, src, exp) for s, e, src, exp in info['units'] for o in ['build/%s.o' % src]}
    units = []
    hand, rem = handwritten(), remnants()
    for pv, obj in info['text_pieces']:
        if obj in c_units:
            s, src, exp = c_units[obj]
            text = open(os.path.join(ROOT, src)).read()
            u = {'name': os.path.splitext(src)[0], 'target_path': exp, 'base_path': obj,
                 'metadata': {'source_path': src, 'complete': 'INCLUDE_ASM' not in text,
                              'progress_categories': [region(s)]}}
            maps = mangled_mappings(obj)
            if maps:
                u['symbol_mappings'] = maps
        else:
            u = {'name': 'asm/text_%08X' % pv, 'target_path': obj,
                 'metadata': {'complete': False,
                              'progress_categories': ['handwritten' if pv in hand else 'remnant' if pv in rem
                                                          else region(pv)]}}
        units.append(u)
    cfg = {
        '$schema': 'https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json',
        'custom_make': 'python', 'custom_args': ['-m', 'ninja'],
        'build_target': False, 'build_base': True,
        'watch_patterns': ['*.c', '*.cpp', '*.h', '*.s'],
        'progress_categories': [{'id': n, 'name': n} for _, n in REGIONS] + [{'id': n, 'name': n} for n in ('handwritten', 'remnant')],
        'units': units,
    }
    with open(os.path.join(ROOT, 'objdiff.json'), 'w', newline='\n') as f:
        json.dump(cfg, f, indent=1)
        f.write('\n')
    os.makedirs(os.path.join(ROOT, 'progress'), exist_ok=True)
    out = os.path.join('progress', 'report.json')
    run([OBJDIFF, 'report', 'generate', '-p', '.', '-o', out, '-f', 'json-pretty'])
    rep = json.load(open(os.path.join(ROOT, out)))
    m = rep.get('measures', {})
    print('report: %s' % out)
    for k in ('matched_code_percent', 'matched_functions', 'total_functions', 'complete_units',
              'total_units', 'matched_code', 'total_code'):
        if k in m:
            print('  %-22s %s' % (k, m[k]))
    desc = {'game': 'game code', 'engine': 'engine (same toolchain as the game)',
            'libgcc': 'compiler runtime (library, not decompiled)',
            'sdk': 'Sony SDK + C/math libraries (not decompiled)', 'crt0': 'startup code',
            'handwritten': 'game/engine functions written in assembly (not decompilable)',
            'remnant': 'dead-strip leftovers of removed functions (not decompilable)'}
    table = ['| Region | What | Code matched | Functions |', '|---|---|---|---|']
    for c in rep.get('categories', []):
        cm = c.get('measures', {})
        print('  %-8s %6.2f%% code, %s/%s functions' % (
            c.get('id'), float(cm.get('matched_code_percent', 0)), cm.get('matched_functions', 0),
            cm.get('total_functions', 0)))
        table.append('| %s | %s | %.2f%% | %s / %s |' % (
            c.get('id'), desc.get(c.get('id'), ''), float(cm.get('matched_code_percent', 0)),
            cm.get('matched_functions', 0), cm.get('total_functions', 0)))
    # what can actually be decompiled: game + engine (handwritten and remnant pieces are their own categories)
    tgt = [c.get('measures', {}) for c in rep.get('categories', []) if c.get('id') in ('game', 'engine')]
    tc, mc = sum(int(x.get('total_code', 0)) for x in tgt), sum(int(x.get('matched_code', 0)) for x in tgt)
    tf, mf = sum(int(x.get('total_functions', 0)) for x in tgt), sum(int(x.get('matched_functions', 0)) for x in tgt)
    if tc:
        print('  target   %6.2f%% code, %s/%s functions (game + engine)' % (100.0 * mc / tc, mf, tf))
        table.append('| **target** | **game + engine: everything there is to decompile** | **%.2f%%** | **%s / %s** |'
                     % (100.0 * mc / tc, mf, tf))
    table.append('| **total** | whole executable, libraries included | **%.2f%%** | **%s / %s** |' % (
        float(m.get('matched_code_percent', 0)), m.get('matched_functions', 0), m.get('total_functions', 0)))
    readme = os.path.join(ROOT, 'README.md')
    text = open(readme, encoding='utf-8').read()
    text = re.sub(r'<!-- progress:start -->.*?<!-- progress:end -->',
                  lambda _: '<!-- progress:start -->\n' + '\n'.join(table) + '\n<!-- progress:end -->',
                  text, flags=re.S)
    with open(readme, 'w', encoding='utf-8', newline='\n') as f:
        f.write(text)


if __name__ == '__main__':
    import srclock
    with srclock.exclusive():
        main()
