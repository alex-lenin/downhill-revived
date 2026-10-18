"""Synthetic native-GS upload/copy ordering checks (D3D11, no game data)."""
from pathlib import Path
import json
import os
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/test-native-transfers'


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    shader = (ROOT / 'port/gs/raster.hlsl').read_text()
    (OUT / 'gs_shader.h').write_text(
        'static const char gs_shader[] = ' + json.dumps(shader) + ';\n'
        'static const char gs_shader_path[] = "port/gs/raster.hlsl";\n')
    clang = shutil.which('clang') or str(Path.home() / 'scoop/apps/llvm/current/bin/clang.exe')
    exe = OUT / 'test-native-transfers.exe'
    command = [clang, '--target=i686-pc-windows-msvc', '-O2', '-w',
               '-I', str(OUT), '-I', str(ROOT / 'port/gs')]
    command += [str(ROOT / 'port/gs' / name) for name in
                ('test_native_transfers.c', 'gpu.c', 'gs.c', 'memory.c')]
    command += ['-o', str(exe), '-ld3d11', '-ldxgi', '-ld3dcompiler', '-luser32']
    subprocess.run(command, check=True)
    env = {k: v for k, v in os.environ.items() if not k.startswith('DHD_')}
    return subprocess.call([str(exe)], cwd=ROOT, env=env)


if __name__ == '__main__':
    sys.exit(main())
