"""Build/run focused Win32 text queue and field validation regressions."""
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/r65/text-test'

def main():
    OUT.mkdir(parents=True, exist_ok=True)
    vswhere = r'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
    vs = subprocess.check_output([vswhere, '-latest', '-property', 'installationPath'], text=True).strip()
    cmd = OUT / 'build.cmd'
    cmd.write_text('@set "PATH=' + str(Path(vswhere).parent) + ';%PATH%"\n'
                   '@call "' + vs + '\\VC\\Auxiliary\\Build\\vcvarsall.bat" x86 >nul\n'
                   '@set "PATH=' + os.path.expanduser(r'~\scoop\apps\llvm\current\bin') + ';%PATH%"\n'
                   '@cd /d "' + str(OUT) + '"\n'
                   'clang-cl -m32 /MT /nologo /W4 /O2 /D_CRT_SECURE_NO_WARNINGS /Fe:text-test.exe "' +
                   str(ROOT / 'port/hle/text_input.c') + '" "' + str(ROOT / 'port/hle/test_text_input.c') +
                   '" /link user32.lib kernel32.lib libcmt.lib libvcruntime.lib libucrt.lib oldnames.lib\n'
                   '@if errorlevel 1 exit /b 1\n"' + str(OUT / 'text-test.exe') + '"\n')
    subprocess.run(['cmd', '/c', str(cmd)], cwd=OUT, check=True)

if __name__ == '__main__':
    main()
