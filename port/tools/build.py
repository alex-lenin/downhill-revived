"""PC build, compile stage: transform every src/ unit (port/tools/transform.py) into
build/port/src/ and compile it with clang for 32-bit Windows (port/tools/pctarget.py) into
build/port/obj/.
Prints errors grouped by kind; build/port/errors.json keeps them per file.
usage: python port/tools/build.py [-j N] [--show TEXT] [FILE...]   (needs LLVM's clang on PATH)"""
import argparse, collections, glob, json, os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import data, eefloat, objcache, pctarget, port_source, transform  # noqa: E402
from port_source import apply_patches, align_aggregate_allocas  # noqa: E402

OUT = os.path.join(ROOT, 'build', 'port')
ap = argparse.ArgumentParser()
ap.add_argument('-j', type=int, default=6)
ap.add_argument('--opt', choices=('1', '2'), default='1', help='port-only game optimization level')
ap.add_argument('--source-root', default=os.path.join(ROOT, 'src'), help='optional clean source snapshot')
ap.add_argument('--out', default=OUT, help='private transformed source/object output')
ap.add_argument('--show')
ap.add_argument('--abi-audit', action='store_true', help='prepare unfixed sources for abi_check.py')
ap.add_argument('--scalar-audit', action='store_true', help='prepare effective native sources for strict scalar ABI guards')
ap.add_argument('--abi-manifest', help='candidate permutations used by the scalar guard')
ap.add_argument('files', nargs='*')
a = ap.parse_args()
OUT = os.path.abspath(a.out)
SOURCE_ROOT = os.path.abspath(a.source_root)
ABI_SITES = json.load(open(a.abi_manifest))['sites'] if a.abi_manifest else None

HEADER_OUT = os.path.join(OUT, 'include')
if a.show is None:
    transform.port_headers(os.path.join(os.path.dirname(SOURCE_ROOT), 'include'), HEADER_OUT)

CLANG = ['clang', '--target=' + pctarget.TARGET, '-c', '-O' + a.opt, '-w', '-ferror-limit=0', '-fno-builtin',
         '-fno-strict-aliasing', '-fwrapv', '-fno-delete-null-pointer-checks',
         '-malign-double',  # 64-bit fields 8-aligned in structs, as on the EE
         # single-precision SSE math, no fused multiply-add: results that don't depend on the
         # optimizer or the machine (x87 keeps excess precision), so state hashes compare across PCs
         '-msse2', '-mfpmath=sse', '-ffp-contract=off',
         # Locals start as zero, not as whatever host code left on the stack: the game reads some
         # before writing them, and host leftovers (ASLR'd stack and heap pointers) made two runs of
         # one build differ (finding 075, state trace). Zero is the same on every host.
         '-ftrivial-auto-var-init=zero',
         # gcc 2.95 accepted these; clang makes them errors by default
         '-Wno-error=incompatible-pointer-types', '-Wno-error=int-conversion',
         '-Wno-error=incompatible-function-pointer-types', '-Wno-error=implicit-function-declaration',
         '-Wno-error=pointer-arith',
         '-I', os.path.join(ROOT, 'port', 'include'), '-I', HEADER_OUT,
         '-I', os.path.join(ROOT, 'include')]
# Diagnostic builds: canaries in every game frame catch a local buffer overflow at its
# function's return (the CRT's check ends the process) instead of a crash at a smashed address.
if os.environ.get('DHD_STACK_PROTECT'):
    CLANG.append('-fstack-protector-all')


IMAGE = data.ImageNames()  # globals the PS2 data image defines (port/tools/data.py)

REWRITERS = objcache.file_hashes(os.path.abspath(__file__), eefloat.__file__, port_source.__file__)


def build(path):
    rel = os.path.relpath(path, SOURCE_ROOT).replace(os.sep, '/')
    src = os.path.join(OUT, 'abi', 'src', rel) if a.abi_audit else os.path.join(OUT, 'abi-native' if a.scalar_audit else '', 'src', rel)
    obj = os.path.join(OUT, 'obj', rel + '.o')
    for d in (src, obj):
        os.makedirs(os.path.dirname(d), exist_ok=True)
    cpp = not path.endswith('.c')
    try:
        text = open(path, encoding='utf-8', errors='replace').read()
        if not a.abi_audit:
            text = transform.fix_abi_calls(text, rel, ABI_SITES)
        text = transform.transform(apply_patches(rel, text), cpp, pctarget.LABEL_PREFIX, IMAGE)
    except Exception as e:  # noqa: BLE001 (report and keep going)
        return rel, [(0, 'transform: %s' % e)]
    with open(src, 'w', encoding='utf-8', newline='\n') as f:
        f.write('#include "port.h"\n#line 1 "src/%s"\n%s' % (rel, text))
    if a.abi_audit or a.scalar_audit:
        return rel, []
    # what gcc 2.95 allowed: -fcommon, C tentative definitions (int x; in two units) merged into
    # one object; -Wno-return-mismatch, a bare return in a C value function;
    # -fno-strict-return, a C++ value function ending without its return
    lang = ['-x', 'c++', '-std=gnu++98', '-fno-exceptions', '-fno-rtti', '-fno-strict-return'] if cpp else \
        ['-x', 'c', '-std=gnu89', '-fcommon', '-Wno-return-mismatch']
    front = [f for f in CLANG if f != '-c'] + lang + ['-S', '-emit-llvm', '-Xclang', '-disable-llvm-passes',
                                                      '-include', eefloat.HEADER]
    # the shared object cache: both steps and the IR rewrite between them (this file, eefloat.py)
    cache = objcache.source_key(front + [src, '-o', '-'], '-', [' '.join(CLANG)] + REWRITERS)
    if cache and objcache.fetch(cache, obj):
        return rel, []
    p = subprocess.run(front + [src, '-o', '-'], capture_output=True, text=True,
                       encoding='utf-8', errors='replace')
    errors = [(int(l), m) for l, m in re.findall(r'^[^\n]*?:(\d+):\d+: error: (.*)$', p.stderr, re.M)]
    if p.returncode or errors:
        return rel, errors or [(0, 'clang: %s' % p.stderr.strip()[-200:])]
    try:
        ir = eefloat.rewrite(align_aggregate_allocas(p.stdout))[0]  # EE float semantics (finding 026)
    except ValueError as e:
        return rel, [(0, str(e))]
    p = subprocess.run(CLANG + ['-x', 'ir', '-', '-o', obj], input=ir,
                       capture_output=True, text=True, encoding='utf-8', errors='replace')
    if cache and not p.returncode:
        objcache.store(cache, obj)
    return rel, [] if not p.returncode else [(0, 'clang (IR): %s' % p.stderr.strip()[-200:])]


def kind(msg):
    return re.sub(r'\d+', 'N', re.sub(r"'[^']*'", "'X'", msg))


if a.show is None:
    files = a.files or sorted(glob.glob(os.path.join(SOURCE_ROOT, '**', '*.c*'), recursive=True))
    with ThreadPoolExecutor(a.j) as ex:
        res = dict(ex.map(build, files))
    if not a.files:
        json.dump(res, open(os.path.join(OUT, 'errors.json'), 'w'), indent=0)
else:
    res = json.load(open(os.path.join(OUT, 'errors.json')))
kinds, files_by = collections.Counter(), collections.defaultdict(set)
for f, errs in res.items():
    for _, m in errs:
        kinds[kind(m)] += 1
        files_by[kind(m)].add(f)
clean = sum(1 for e in res.values() if not e)
print('%d files, %d compile, %d with errors, %d errors' % (
    len(res), clean, len(res) - clean, sum(len(e) for e in res.values())))
for k, n in kinds.most_common(30):
    print('%6d %5d files  %s' % (n, len(files_by[k]), k[:140]))
if a.show:
    for f, errs in res.items():
        for l, m in errs:
            if a.show in m:
                print('src/%s:%d: %s' % (f, l, m))

if (a.abi_audit or a.scalar_audit) and clean != len(res):
    raise SystemExit(1)
