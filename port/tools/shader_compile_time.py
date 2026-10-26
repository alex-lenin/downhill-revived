"""Time FXC and DXC on raster.wgsl's compute entry points, as wgpu's DX12 backend compiles them.

    python port/tools/shader_compile_time.py [port/gs/raster.wgsl] [--dxc] [--entry main]

naga (DHD_DEVCACHES/naga, naga-cli 29 to match wgpu-native v29: cargo install naga-cli --version ^29
--root DHD_DEVCACHES/naga) writes each entry point's HLSL with the backend's override constants;
fxc (SM 5.1, wgpu's FXC path) or dxc (SM 6.0) from the Windows SDK compiles it with wgpu's
flags (optimization level 3). Prints seconds per entry point and the HLSL size. This times the
compiler alone, without a device, so shader changes can be measured in seconds (finding 084 §14).
"""
import glob
import os
import subprocess
import sys
import time
from pathlib import Path
DEVCACHES = Path(os.environ.get('DHD_DEVCACHES') or Path.home() / 'DevCaches')  # downloaded tools and caches

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
NAGA = str(DEVCACHES / 'naga/bin/naga.exe')
SDK = sorted(glob.glob('C:/Program Files (x86)/Windows Kits/10/bin/10.*/x64'))[-1]
ENTRIES = {'main': 'EARLY_DEPTH=1,ADDRESS_SHIFT=1,EXCLUSIVE_WORD_STORE=1', 'local_copy': 'ADDRESS_SHIFT=1',
           'scanout': '', 'depth_probe': ''}


def main():
    args = sys.argv[1:]
    dxc = '--dxc' in args
    only = args[args.index('--entry') + 1] if '--entry' in args else None
    files = [a for a in args if a.endswith('.wgsl')] or [os.path.join(ROOT, 'port/gs/raster.wgsl')]
    out = os.path.join(ROOT, 'build/shader-time'); os.makedirs(out, exist_ok=True)
    total = 0.0
    for name, overrides in ENTRIES.items():
        if only and name != only:
            continue
        hlsl = os.path.join(out, name + '.hlsl')
        cmd = [NAGA, '--entry-point', name, '--compact', '--shader-model', '60' if dxc else '51']
        if overrides:
            cmd += ['--override', overrides]
        r = subprocess.run(cmd + [files[0], hlsl], capture_output=True, text=True)
        if r.returncode:
            sys.exit(r.stdout + r.stderr)
        if dxc:
            cmd = [SDK + '/dxc.exe', '-T', 'cs_6_0', '-E', name, '-O3', '-HV', '2018', hlsl, '-Fo', hlsl + '.dxil']
        else:
            cmd = [SDK + '/fxc.exe', '/nologo', '/T', 'cs_5_1', '/E', name, '/O3', hlsl, '/Fo', hlsl + '.dxbc']
        t = time.perf_counter()
        r = subprocess.run(cmd, capture_output=True, text=True)
        seconds = time.perf_counter() - t
        total += seconds
        if r.returncode:
            sys.exit(r.stdout + r.stderr)
        print('%-12s %7.2f s  (%d lines of HLSL)' % (name, seconds, sum(1 for _ in open(hlsl))))
    print('%-12s %7.2f s  (%s)' % ('total', total, 'dxc' if dxc else 'fxc'))


if __name__ == '__main__':
    main()
