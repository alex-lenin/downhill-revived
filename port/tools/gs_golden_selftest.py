"""Gate G0's referee of port/tools/gs_golden.py (the Rust renderer plan, sections 4.2 G0 and 5.2):
planted mutations that compare must fail or refuse, and unchanged copies that compare must pass.

Changes to this file may only add conditions: no check is ever relaxed.

    python port/tools/gs_golden_selftest.py [--golden GOLD] [--synthetic-only]

Every case runs gs_golden.py's command line in a child process and judges its exit code and output,
so what is tested is exactly what a referee sees. Three parts:

1. gs_golden.py selftest (the recorder's own mutations) must pass.
2. Synthetic: a golden of hand-made files (no game data) in a scratch folder (DHD_SCRATCH, else the system's temp), one item of every kind
   compare reads (a DHD_GS_VRAM_LOG, PPM pictures, vram.txt / vram.bin, native-transfer .name /
   .rgba / .vram dumps, web-vram.txt), with a meta.json pinned to this PC's first PCI adapter, and
   retired d3d11 items (gs_golden.RETIRED: in the golden, never in a run). Each case copies it,
   without the retired items, as a record directory (or as a harness's output layout) and plants one
   change:
     must fail:   a VRAM word (log, vram.bin, .vram), a pixel (PPM, RGBA), an alpha-only RGBA byte,
                  a picture header, a changed hash line, a missing file, an extra file, a truncated
                  file, a CR-only difference in a binary file, a missing group, a meta count that
                  disagrees, an empty or absent source (zero-item glob) for every source layout,
                  a retired d3d11 item in a run (record, test-gpu, two-backend replay and
                  native-transfers layouts);
     must refuse: the run's adapter or driver, the golden's device or driver, the WARP version,
                  a golden file changed after recording (a retired one included), a golden file
                  missing or extra, a golden without meta.json or of another format;
     must pass:   the unchanged copy (all and per group, in each layout) and a CR-only difference
                  in vram.txt and web-vram.txt.
3. Real (skipped with --synthetic-only): copies of $GOLD's small groups (this PC's golden, or
   --golden), compared unchanged (must pass: the golden is intact and pinned to this PC) and with one
   planted change each (must fail): test-gpu (a VRAM word; the wgpu logs only, as test_gs_wgpu.py
   writes them), replay g+0 in replay_gs's layout (a pixel, a vram.bin word, a hash line), web g+0 (a
   hash digit; CR-only must pass).
   The copies stay in the scratch folder and are deleted afterwards; the golden is only read.

Exit 0 when every case behaves, 1 otherwise."""
import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gs_golden  # noqa: E402
import gs_verdict  # noqa: E402

TOOL = Path(__file__).resolve().with_name('gs_golden.py')
SCRATCH = Path(os.environ['DHD_SCRATCH']) if os.environ.get('DHD_SCRATCH') else None
STEP = gs_verdict.CHECKPOINT_NAME + gs_verdict.VRAM_BYTES


def tool(*args):
    """(exit code, output) of gs_golden.py with these arguments."""
    p = subprocess.run([sys.executable, str(TOOL), *[str(a) for a in args]], capture_output=True, text=True,
                       timeout=1800)
    return p.returncode, p.stdout + p.stderr


class Cases:
    def __init__(self):
        self.failures = []
        self.count = 0

    def check(self, name, args, expect, needles=()):
        """expect: 'pass' (exit 0 and VERDICT PASS), 'fail' (exit 1, VERDICT FAIL, not refused) or
        'refuse' (exit 1, REFUSED)."""
        code, text = tool(*args)
        self.count += 1
        if expect == 'pass':
            ok = code == 0 and 'VERDICT PASS' in text and 'VERDICT FAIL' not in text
        elif expect == 'fail':
            ok = code == 1 and 'VERDICT FAIL' in text and 'REFUSED' not in text
        else:
            ok = code == 1 and 'REFUSED' in text and 'VERDICT FAIL' in text
        missing = [n for n in needles if n not in text]
        ok = ok and not missing
        print('%-34s exit %d, expected %-6s %s' % (name, code, expect, 'ok' if ok else '<-- WRONG'))
        if not ok:
            self.failures.append(name)
            if missing:
                print('    missing in the output: %s' % missing)
            for line in text.strip().splitlines()[-25:]:
                print('    ' + line)


# ---------------------------------------------------------------- files and mutations

def flip(path, offset, mask=0x01):
    data = bytearray(Path(path).read_bytes())
    data[offset] ^= mask
    os.chmod(path, 0o666)
    Path(path).write_bytes(bytes(data))


def rewrite(path, function):
    os.chmod(path, 0o666)
    Path(path).write_bytes(function(Path(path).read_bytes()))


def copy_tree(src, dst):
    shutil.copytree(src, dst)
    for p in Path(dst).rglob('*'):
        if p.is_file():
            os.chmod(p, 0o666)


def ppm(w, h, seed):
    return b'P6\n%d %d\n255\n' % (w, h) + bytes((seed + i * 7) & 255 for i in range(w * h * 3))


def synthetic_files():
    """{group/rel: bytes}: one item of every kind compare reads (no game data)."""
    log = b''
    for n, name in enumerate((b'clear', b'sprite')):
        log += name.ljust(gs_verdict.CHECKPOINT_NAME, b'\0') + bytes((n * 37 + i) & 255 for i in range(256)) * (
            gs_verdict.VRAM_BYTES // 256)
    rgba = bytes((i * 5) & 255 for i in range(4 * 4 * 4)) + (4).to_bytes(4, 'little') + (4).to_bytes(4, 'little')
    files = {
        'test-gpu/vram-hardware-wgpu.bin': log,
        'test-gpu/vram-hardware-d3d11.bin': log,  # retired (d3d11), as are the d3d11 items below
        'replay/g+0/wgpu/000.ppm': ppm(4, 2, 1),
        'replay/g+0/wgpu/001.ppm': ppm(4, 2, 2),
        'replay/g+0/wgpu/vram.txt': b'0 0123456789ABCDEF\n1 FEDCBA9876543210\n',
        'replay/g+0/wgpu/vram.bin': bytes(range(256)) * 64,
        'replay/g+0/d3d11/000.ppm': ppm(4, 2, 1),
        'replay/g+0/d3d11/001.ppm': ppm(4, 2, 2),
        'replay/g+0/d3d11/vram.txt': b'0 0123456789ABCDEF\n1 FEDCBA9876543210\n',
        'replay/g+0/d3d11/vram.bin': bytes(range(256)) * 64,
        'native-transfers/wgpu/00.name': b'upload after clear',
        'native-transfers/wgpu/00.rgba': rgba,
        'native-transfers/wgpu/00.vram': bytes(range(256)) * 32,
        'native-transfers/d3d11/00.name': b'upload after clear',
        'native-transfers/d3d11/00.rgba': rgba,
        'native-transfers/d3d11/00.vram': bytes(range(256)) * 32,
        'web/g+0/web-vram.txt': b'0 0123456789ABCDEF\n1 FEDCBA9876543210\n',
    }
    return files


def live_files(files):
    """The items a run has: the golden's without the retired ones."""
    return {rel: data for rel, data in files.items() if not gs_golden.retired(*rel.split('/', 1))}


def write_record(directory, files, golden, adapter, warp):
    meta = {'format': gs_golden.FORMAT, 'golden': golden, 'adapters': {'exact': adapter}, 'warp': warp,
            'groups': {}, 'count': 0}
    for rel, data in files.items():
        p = directory / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(data)
        group, item = rel.split('/', 1)
        g = meta['groups'].setdefault(group, {'count': 0, 'items': {}})
        g['items'][item] = {'sha256': hashlib.sha256(data).hexdigest(), 'size': len(data)}
        g['count'] += 1
        meta['count'] += 1
    (directory / 'meta.json').write_text(json.dumps(meta), encoding='utf-8')
    return meta


def edit_meta(directory, function):
    path = directory / 'meta.json'
    os.chmod(path, 0o666)
    meta = json.loads(path.read_text(encoding='utf-8'))
    function(meta)
    path.write_text(json.dumps(meta), encoding='utf-8')


def synthetic(cases, root):
    system = gs_golden.system_info()
    adapter = gs_golden.default_adapter(system)
    files = synthetic_files()
    live = live_files(files)
    total = len(live)
    golden = root / 'golden'

    def fresh_golden():
        shutil.rmtree(golden, ignore_errors=True)
        write_record(golden, files, True, adapter, system['warp'])

    fresh_golden()
    serial = [0]

    def run(mutate=None):
        serial[0] += 1
        d = root / ('run%02d' % serial[0])
        write_record(d, live, False, adapter, system['warp'])
        if mutate:
            mutate(d)
        return d

    def layout(kind, mutate=None):
        """The run in a harness's own output layout (not a record): (source argument, extra args).
        'replay' is the goldens' two-backend layout, with the retired d3d11 items (must fail)."""
        serial[0] += 1
        d = root / ('out%02d' % serial[0])
        d.mkdir()
        if kind == 'test-gpu':
            for rel, data in live.items():
                if rel.startswith('test-gpu/'):
                    (d / rel.split('/', 1)[1]).write_bytes(data)
            source = d
        elif kind == 'replay':  # the retired replay_gs.py --backend both: PREFIX-<backend>-NNN.ppm, ...
            for rel, data in files.items():
                if rel.startswith('replay/g+0/'):
                    backend, name = rel.split('/')[2:]
                    (d / ('p-%s-%s' % (backend, name))).write_bytes(data)
            source = d / 'p'
        elif kind == 'replay-one':  # one backend: PREFIX-NNN.ppm ...
            for rel, data in files.items():
                if rel.startswith('replay/g+0/wgpu/'):
                    (d / ('p-' + rel.rsplit('/', 1)[1])).write_bytes(data)
            source = d / 'p'
        elif kind == 'native-transfers':  # test_native_transfers.py: OUT/dump/<run>/
            for rel, data in live.items():
                if rel.startswith('native-transfers/'):
                    p = d / 'dump' / rel.split('/', 1)[1]
                    p.parent.mkdir(parents=True, exist_ok=True)
                    p.write_bytes(data)
            source = d
        elif kind == 'web':
            (d / 'p-web-vram.txt').write_bytes(files['web/g+0/web-vram.txt'])
            source = d / 'p'
        else:
            raise ValueError(kind)
        if mutate:
            mutate(d)
        return source

    G = ['--golden', golden]
    log_word = gs_verdict.CHECKPOINT_NAME + 4 * 1000 + STEP  # a word of the second checkpoint
    # ---- the unchanged copies pass
    cases.check('identical (all)', ['compare', 'all', run()] + G, 'pass',
                ['compared %d items (meta count %d): identical' % (total, total)])
    cases.check('identical test-gpu layout', ['compare', 'test-gpu', layout('test-gpu')] + G, 'pass',
                ['compared 1 items (meta count 1)'])
    cases.check('identical replay one backend', ['compare', 'replay', layout('replay-one')] + G, 'pass',
                ['compared 4 items (meta count 4)'])
    cases.check('identical native-transfers', ['compare', 'native-transfers', layout('native-transfers')] + G,
                'pass', ['compared 3 items (meta count 3)'])
    cases.check('identical web', ['compare', 'web', layout('web')] + G, 'pass', ['compared 1 items (meta count 1)'])
    # ---- CR-only differences pass in text files only
    crlf = lambda data: data.replace(b'\n', b'\r\n')  # noqa: E731
    cr = lambda data: data.replace(b'\n', b'\r')  # noqa: E731
    cases.check('cr-only vram.txt (CRLF)', ['compare', 'all', run(lambda d: rewrite(d / 'replay/g+0/wgpu/vram.txt', crlf))] + G,
                'pass')
    cases.check('cr-only vram.txt (CR)', ['compare', 'replay', layout('replay-one', lambda d: rewrite(d / 'p-vram.txt', cr))] + G,
                'pass')
    cases.check('cr-only web-vram.txt', ['compare', 'web', layout('web', lambda d: rewrite(d / 'p-web-vram.txt', crlf))] + G,
                'pass')
    cases.check('cr-only in a binary (.bin)', ['compare', 'all', run(
        lambda d: rewrite(d / 'replay/g+0/wgpu/vram.bin', lambda b: b.replace(b'\n', b'\r\n')))] + G, 'fail')
    cases.check('cr-only in a picture (.ppm)', ['compare', 'all', run(
        lambda d: rewrite(d / 'replay/g+0/wgpu/000.ppm', crlf))] + G, 'fail')
    # ---- one changed word, pixel or line fails
    cases.check('vram word (test-gpu log)', ['compare', 'all', run(lambda d: flip(d / 'test-gpu/vram-hardware-wgpu.bin', log_word))] + G,
                'fail', ['1 words differ'])
    cases.check('vram word (test-gpu layout)', ['compare', 'test-gpu', layout('test-gpu', lambda d: flip(d / 'vram-hardware-wgpu.bin', log_word, 0x80))] + G,
                'fail', ['1 words differ'])
    cases.check('checkpoint name (test-gpu)', ['compare', 'all', run(lambda d: flip(d / 'test-gpu/vram-hardware-wgpu.bin', STEP + 1, 0x20))] + G,
                'fail', ['checkpoint names differ'])
    cases.check('vram word (replay vram.bin)', ['compare', 'replay', layout('replay-one', lambda d: flip(d / 'p-vram.bin', 1000))] + G,
                'fail', ['1 words differ'])
    cases.check('vram word (transfers .vram)', ['compare', 'native-transfers', layout('native-transfers', lambda d: flip(d / 'dump/wgpu/00.vram', 4095))] + G,
                'fail', ['1 words differ'])
    cases.check('pixel (ppm)', ['compare', 'all', run(lambda d: flip(d / 'replay/g+0/wgpu/001.ppm', len(b'P6\n4 2\n255\n') + 3 * 5 + 2))] + G,
                'fail', ['1 pixels differ'])
    cases.check('pixel (ppm, one backend)', ['compare', 'replay', layout('replay-one', lambda d: flip(d / 'p-000.ppm', len(b'P6\n4 2\n255\n')))] + G,
                'fail', ['1 pixels differ'])
    cases.check('pixel (rgba)', ['compare', 'native-transfers', layout('native-transfers', lambda d: flip(d / 'dump/wgpu/00.rgba', 4 * 9 + 1))] + G,
                'fail', ['1 pixels differ'])
    cases.check('alpha byte only (rgba)', ['compare', 'all', run(lambda d: flip(d / 'native-transfers/wgpu/00.rgba', 4 * 3 + 3))] + G,
                'fail', ['0 pixels differ'])
    cases.check('picture header (ppm size)', ['compare', 'all', run(lambda d: rewrite(d / 'replay/g+0/wgpu/000.ppm', lambda b: b.replace(b'4 2', b'2 4', 1)))] + G,
                'fail')
    cases.check('picture size (rgba trailer)', ['compare', 'all', run(lambda d: flip(d / 'native-transfers/wgpu/00.rgba', 4 * 16))] + G,
                'fail', ['picture size differs'])
    cases.check('scene name (.name)', ['compare', 'all', run(lambda d: rewrite(d / 'native-transfers/wgpu/00.name', lambda b: b + b'!'))] + G,
                'fail')
    cases.check('hash line (vram.txt)', ['compare', 'all', run(lambda d: rewrite(d / 'replay/g+0/wgpu/vram.txt', lambda b: b.replace(b'CDEF', b'CDEE')))] + G,
                'fail', ['1 lines differ'])
    cases.check('hash line (web)', ['compare', 'web', layout('web', lambda d: rewrite(d / 'p-web-vram.txt', lambda b: b.replace(b'3210', b'3211')))] + G,
                'fail', ['1 lines differ'])
    cases.check('extra line (vram.txt)', ['compare', 'all', run(lambda d: rewrite(d / 'replay/g+0/wgpu/vram.txt', lambda b: b + b'2 0000000000000000\n'))] + G,
                'fail')
    # ---- missing, extra, truncated
    cases.check('missing file (all)', ['compare', 'all', run(lambda d: (d / 'replay/g+0/wgpu/vram.bin').unlink())] + G,
                'fail', ['missing'])
    cases.check('missing picture (replay layout)', ['compare', 'replay', layout('replay-one', lambda d: (d / 'p-001.ppm').unlink())] + G,
                'fail', ['missing'])
    cases.check('missing dump (transfers)', ['compare', 'native-transfers', layout('native-transfers', lambda d: (d / 'dump/wgpu/00.vram').unlink())] + G,
                'fail', ['missing'])
    cases.check('extra file (all)', ['compare', 'all', run(lambda d: (d / 'replay/g+0/wgpu/002.ppm').write_bytes(ppm(4, 2, 3)))] + G,
                'fail', ['extra'])
    cases.check('extra picture (replay layout)', ['compare', 'replay', layout('replay-one', lambda d: (d / 'p-002.ppm').write_bytes(ppm(4, 2, 3)))] + G,
                'fail', ['extra'])
    cases.check('extra dump (transfers)', ['compare', 'native-transfers', layout('native-transfers', lambda d: (d / 'dump/wgpu/01.name').write_bytes(b'x'))] + G,
                'fail', ['extra'])
    cases.check('truncated vram.bin', ['compare', 'all', run(lambda d: rewrite(d / 'replay/g+0/wgpu/vram.bin', lambda b: b[:-4]))] + G,
                'fail', ['truncated'])
    cases.check('truncated vram log', ['compare', 'all', run(lambda d: rewrite(d / 'test-gpu/vram-hardware-wgpu.bin', lambda b: b[:-STEP]))] + G,
                'fail')
    cases.check('truncated vram.txt', ['compare', 'all', run(lambda d: rewrite(d / 'replay/g+0/wgpu/vram.txt', lambda b: b[:-1]))] + G,
                'fail')
    cases.check('empty picture', ['compare', 'all', run(lambda d: rewrite(d / 'replay/g+0/wgpu/001.ppm', lambda b: b''))] + G,
                'fail')
    cases.check('missing group (all)', ['compare', 'all', run(lambda d: shutil.rmtree(d / 'web'))] + G, 'fail')
    # ---- a retired (d3d11) item in a run fails, in every layout
    def add(rel, data):
        def f(d):
            (d / rel).parent.mkdir(parents=True, exist_ok=True)
            (d / rel).write_bytes(data)
        return f
    cases.check('retired log (all)', ['compare', 'all', run(add('test-gpu/vram-hardware-d3d11.bin', files['test-gpu/vram-hardware-d3d11.bin']))] + G,
                'fail', ['retired'])
    cases.check('retired dump (all)', ['compare', 'all', run(add('native-transfers/d3d11/00.name', files['native-transfers/d3d11/00.name']))] + G,
                'fail', ['retired'])
    cases.check('retired log (test-gpu layout)', ['compare', 'test-gpu', layout('test-gpu', add('vram-hardware-d3d11.bin', files['test-gpu/vram-hardware-d3d11.bin']))] + G,
                'fail', ['retired'])
    cases.check('retired replay (two-backend layout)', ['compare', 'replay', layout('replay')] + G, 'fail', ['retired'])
    cases.check('retired dump (transfers layout)', ['compare', 'native-transfers', layout('native-transfers', add('dump/d3d11/00.name', files['native-transfers/d3d11/00.name']))] + G,
                'fail', ['retired'])
    # ---- zero-item sources fail
    empty = root / 'empty'
    empty.mkdir()
    cases.check('zero items: transfers dir', ['compare', 'native-transfers', empty] + G, 'fail', ['no items'])
    cases.check('zero items: test-gpu dir', ['compare', 'test-gpu', empty] + G, 'fail', ['no items'])
    cases.check('zero items: replay prefix', ['compare', 'replay', empty / 'nothing'] + G, 'fail', ['no items'])
    cases.check('zero items: web prefix', ['compare', 'web', empty / 'nothing'] + G, 'fail', ['no items'])
    cases.check('zero items: run group', ['compare', 'replay', run(lambda d: shutil.rmtree(d / 'replay'))] + G,
                'fail', ['no items'])
    cases.check('zero items: absent dir', ['compare', 'native-transfers', root / 'absent'] + G, 'fail', ['no items'])
    # ---- the run's meta
    def other_driver(meta):
        meta['adapters']['exact'] = dict(meta['adapters']['exact'], driver='0.0.0.1')
    cases.check('run adapter driver', ['compare', 'all', run(lambda d: edit_meta(d, other_driver))] + G, 'refuse')
    cases.check('run adapter driver (one group)', ['compare', 'replay', run(lambda d: edit_meta(d, other_driver))] + G, 'refuse')
    cases.check('run warp', ['compare', 'all', run(lambda d: edit_meta(d, lambda m: m.update(warp='0.0.0.0')))] + G, 'refuse')
    cases.check('run meta missing (all)', ['compare', 'all', run(lambda d: (d / 'meta.json').unlink())] + G, 'refuse')
    # ---- the golden itself
    edit_meta(golden, other_driver)
    cases.check('golden driver', ['compare', 'all', run(lambda d: edit_meta(d, other_driver))] + G, 'refuse')
    cases.check('golden driver (layout)', ['compare', 'web', layout('web')] + G, 'refuse')
    fresh_golden()
    edit_meta(golden, lambda m: m['adapters'].update(exact=dict(m['adapters']['exact'], device='ffff')))
    cases.check('golden device', ['compare', 'native-transfers', layout('native-transfers')] + G, 'refuse')
    fresh_golden()
    edit_meta(golden, lambda m: m['adapters'].update(native=dict(m['adapters']['exact'], name='No Such Adapter')))
    cases.check('golden adapter absent', ['compare', 'replay', layout('replay-one')] + G, 'refuse')
    fresh_golden()
    edit_meta(golden, lambda m: m.update(warp='0.0.0.0'))
    cases.check('golden warp (test-gpu)', ['compare', 'test-gpu', layout('test-gpu')] + G, 'refuse')
    fresh_golden()
    flip(golden / 'replay/g+0/wgpu/vram.bin', 5)
    cases.check('golden file changed', ['compare', 'all', run()] + G, 'refuse')
    fresh_golden()
    flip(golden / 'replay/g+0/d3d11/vram.bin', 5)
    cases.check('golden retired file changed', ['compare', 'replay', layout('replay-one')] + G, 'refuse')
    fresh_golden()
    (golden / 'native-transfers/d3d11/00.vram').unlink()
    cases.check('golden retired file missing', ['compare', 'native-transfers', layout('native-transfers')] + G, 'refuse')
    fresh_golden()
    (golden / 'native-transfers/wgpu/00.vram').unlink()
    cases.check('golden file missing', ['compare', 'native-transfers', layout('native-transfers')] + G, 'refuse')
    fresh_golden()
    (golden / 'web/g+0/extra.txt').write_bytes(b'x')
    cases.check('golden file extra', ['compare', 'web', layout('web')] + G, 'refuse')
    fresh_golden()
    edit_meta(golden, lambda m: m.update(format=gs_golden.FORMAT + 1))
    cases.check('golden format', ['compare', 'all', run()] + G, 'refuse')
    fresh_golden()
    (golden / 'meta.json').unlink()
    cases.check('golden without meta.json', ['compare', 'all', run()] + G, 'refuse')
    fresh_golden()
    edit_meta(golden, lambda m: m.update(count=m['count'] + 1))
    cases.check('meta count mismatch', ['compare', 'all', run()] + G, 'fail', ['meta count %d' % (total + 1)])
    fresh_golden()


def real(cases, root, gold):
    """Copies of the recorded golden's small groups: unchanged must pass, one planted change must fail."""
    meta = gs_golden.load_meta(gold)
    if meta is None:
        print('real: %s has no meta.json' % gold)
        cases.failures.append('real golden missing')
        return
    G = ['--golden', gold]
    groups = meta['groups']
    # test-gpu: the directory layout of test_gs_wgpu.py (its wgpu logs; the d3d11 ones are retired)
    d = root / 'real-test-gpu'
    d.mkdir()
    logs = sorted(rel for rel in groups['test-gpu']['items'] if not gs_golden.retired('test-gpu', rel))
    for rel in logs:
        shutil.copy2(gold / 'test-gpu' / rel, d / rel)
        os.chmod(d / rel, 0o666)
    n = len(logs)
    cases.check('real test-gpu unchanged', ['compare', 'test-gpu', d] + G, 'pass',
                ['compared %d items (meta count %d): identical' % (n, n)])
    first = logs[0]
    flip(d / first, gs_verdict.CHECKPOINT_NAME + 4 * 4096 + 2)
    cases.check('real test-gpu vram word', ['compare', 'test-gpu', d] + G, 'fail', ['1 words differ'])
    # replay g+0 wgpu, replay_gs.py's one-backend layout
    d = root / 'real-replay'
    d.mkdir()
    items = sorted(rel for rel in groups['replay']['items'] if rel.startswith('g+0/wgpu/'))
    for rel in items:
        shutil.copy2(gold / 'replay' / rel, d / ('p-' + rel.rsplit('/', 1)[1]))
        os.chmod(d / ('p-' + rel.rsplit('/', 1)[1]), 0o666)
    one = ['compare', 'replay', d / 'p', '--backend', 'wgpu', '--capture', 'g+0'] + G
    cases.check('real replay g+0 unchanged', one, 'pass',
                ['compared %d items (meta count %d): identical' % (len(items), len(items))])
    pictures = sorted(d.glob('p-[0-9][0-9][0-9].ppm'))
    header = len(b'\n'.join(pictures[-1].read_bytes().split(b'\n', 3)[:3])) + 1
    flip(pictures[-1], header + 3 * 1000 + 1)
    cases.check('real replay pixel', one, 'fail', ['1 pixels differ'])
    flip(pictures[-1], header + 3 * 1000 + 1)
    flip(d / 'p-vram.bin', 4 * 123456 + 3, 0x40)
    cases.check('real replay vram.bin word', one, 'fail', ['1 words differ'])
    flip(d / 'p-vram.bin', 4 * 123456 + 3, 0x40)
    rewrite(d / 'p-vram.txt', lambda b: b[:-2] + bytes([b[-2] ^ 1]) + b[-1:])
    cases.check('real replay hash line', one, 'fail', ['1 lines differ'])
    # web g+0
    d = root / 'real-web'
    d.mkdir()
    shutil.copy2(gold / 'web/g+0/web-vram.txt', d / 'p-web-vram.txt')
    os.chmod(d / 'p-web-vram.txt', 0o666)
    web = ['compare', 'web', d / 'p'] + G
    cases.check('real web unchanged', web, 'pass', ['compared 1 items (meta count 1): identical'])
    rewrite(d / 'p-web-vram.txt', lambda b: b.replace(b'\r\n', b'\n').replace(b'\n', b'\r\n'))
    cases.check('real web cr-only', web, 'pass')
    rewrite(d / 'p-web-vram.txt', lambda b: b.replace(b'\r\n', b'\n'))
    lines = (d / 'p-web-vram.txt').read_bytes().split(b'\n')
    lines[0] = lines[0][:-1] + (b'0' if lines[0][-1:] != b'0' else b'1')
    rewrite(d / 'p-web-vram.txt', lambda b: b'\n'.join(lines))
    cases.check('real web hash digit', web, 'fail', ['1 lines differ'])


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--golden', help="the recorded golden (default: this PC's, gs_golden.py key)")
    ap.add_argument('--synthetic-only', action='store_true', help='skip the cases on copies of the real golden')
    args = ap.parse_args(argv)
    cases = Cases()
    code, text = tool('selftest')
    print(text.rstrip())
    cases.count += 1
    if code != 0 or 'selftest: PASS' not in text:
        cases.failures.append('gs_golden.py selftest')
    root = Path(tempfile.mkdtemp(prefix='gs-golden-g0-', dir=SCRATCH))
    try:
        synthetic(cases, root)
        if not args.synthetic_only:
            gold = Path(args.golden) if args.golden else gs_golden.GOLDEN_ROOT / gs_golden.adapter_key(gs_golden.default_adapter())
            real(cases, root, gold)
    finally:
        shutil.rmtree(root, ignore_errors=True)
    print('gs_golden_selftest: %d cases, %s' % (cases.count, 'PASS' if not cases.failures else
                                                 'FAIL ' + ', '.join(cases.failures)))
    return 1 if cases.failures else 0


if __name__ == '__main__':
    sys.exit(main())
