"""Replay a native GS capture without the EE/VU. Outputs stay under build/.

Usage: python port/tools/replay_gs.py capture.gs [output-prefix]
Requires LLVM and Visual Studio x86 libraries, like test_runtime.py.
"""
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/gs-replay'


def main():
    if len(sys.argv) not in (2, 3):
        sys.exit(__doc__)
    capture = Path(sys.argv[1]).resolve()
    prefix = Path(sys.argv[2]).resolve() if len(sys.argv) == 3 else OUT / capture.stem
    if not prefix.is_relative_to(ROOT / 'build'):
        sys.exit('Replay output must be under build/')
    OUT.mkdir(parents=True, exist_ok=True)
    prefix.parent.mkdir(parents=True, exist_ok=True)
    gs = ROOT / 'port/gs'
    shader = (gs / 'raster.hlsl').read_text(encoding='utf-8')
    (OUT / 'gs_shader.h').write_text('static const char gs_shader[] = ' + json.dumps(shader) +
        ';\nstatic const char gs_shader_path[] = ' + json.dumps(str(gs / 'raster.hlsl')) + ';\n')
    vswhere = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe')
    vs = subprocess.check_output([str(vswhere), '-latest', '-property', 'installationPath'], text=True).strip()
    sources = ' '.join('"%s"' % (gs / (name + '.c')) for name in ('memory','gs','display','gpu','replay'))
    exe = OUT / 'replay.exe'
    cmd = OUT / 'replay.cmd'
    cmd.write_text('@set "PATH=%%PATH%%;%s"\n'
        '@call "%s" x86 >nul\n@if errorlevel 1 exit /b 1\n'
        'clang-cl -m32 /nologo /O2 -Wno-deprecated-declarations /I"%s" %s /Fe:"%s" '
        '/link d3d11.lib d3dcompiler.lib\n' %
        (vswhere.parent, Path(vs) / 'VC/Auxiliary/Build/vcvarsall.bat', OUT, sources, exe))
    subprocess.run(['cmd', '/c', str(cmd)], cwd=OUT, check=True)
    subprocess.run([str(exe), str(capture), str(prefix)], cwd=OUT, check=True)


if __name__ == '__main__':
    main()
