"""Build/run focused Win32 text queue and field validation regressions."""
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/r65/text-test'
sys.path.insert(0, str(ROOT / 'port/tools'))

def main():
    import sdl3  # text_input.c reads the clipboard through SDL (W11)
    sdl_lib, sdl_include = sdl3.build('i686-pc-windows-msvc')  # a 32-bit test program (-m32)
    OUT.mkdir(parents=True, exist_ok=True)
    vswhere = r'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
    vs = subprocess.check_output([vswhere, '-latest', '-property', 'installationPath'], text=True).strip()
    cmd = OUT / 'build.cmd'
    cmd.write_text('@set "PATH=' + str(Path(vswhere).parent) + ';%PATH%"\n'
                   '@call "' + vs + '\\VC\\Auxiliary\\Build\\vcvarsall.bat" x86 >nul\n'
                   '@set "PATH=' + os.path.expanduser(r'~\scoop\apps\llvm\current\bin') + ';%PATH%"\n'
                   '@cd /d "' + str(OUT) + '"\n'
                   'clang-cl -m32 /MT /nologo /W4 /O2 /D_CRT_SECURE_NO_WARNINGS /I"' + str(sdl_include) +
                   '" /Fe:text-test.exe "' + str(ROOT / 'port/hle/text_input.c') + '" "' +
                   str(ROOT / 'port/hle/keyboard.c') + '" "' + str(ROOT / 'port/hle/test_text_input.c') +
                   '" "' + str(sdl_lib) + '" ' + ' '.join(sdl3.SYSTEM_LIBS) +
                   ' /link user32.lib kernel32.lib libcmt.lib libvcruntime.lib libucrt.lib oldnames.lib\n'
                   '@if errorlevel 1 exit /b 1\n"' + str(OUT / 'text-test.exe') + '"\n')
    subprocess.run(['cmd', '/c', str(cmd)], cwd=OUT, check=True)

if __name__ == '__main__':
    main()
