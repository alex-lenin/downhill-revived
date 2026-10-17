"""Build and run the network module's tests as 32-bit Windows programs (clang-cl and Visual
Studio's x86 libraries, as port/tools/test_runtime.py does). Separate from the port's build.
usage: python port/net/build_tests.py [--no-run] [PROGRAM...]"""
import os, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
OUT = os.path.join(ROOT, 'build', 'port', 'net')
FLAGS = '-m32 /nologo /O2 /W4 -Wno-deprecated-declarations /D_CRT_SECURE_NO_WARNINGS'
CORE = ['lockstep.c', 'hash.c']
CLIENT = CORE + ['json.c', 'client.c']
PROGRAMS = {  # name: (sources, libraries, run in the tests)
    'test_lockstep': (CORE + ['test_lockstep.c'], '', True),
    'test_json': (['json.c', 'test_json.c'], '', True),
    'test_loopback': (CLIENT + ['test_loopback.c'], 'winmm.lib', True),
    'nettool': (CLIENT + ['ws_winhttp.c', 'net.c', 'nettool.c'], 'winhttp.lib winmm.lib', False),
}


def main():
    os.makedirs(OUT, exist_ok=True)
    vswhere = r'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
    vs = subprocess.run([vswhere, '-latest', '-property', 'installationPath'], capture_output=True,
                        text=True).stdout.strip()
    llvm = os.path.expanduser(r'~\scoop\apps\llvm\current\bin')
    cmd = os.path.join(OUT, 'build.cmd')
    with open(cmd, 'w') as f:
        f.write('@set "PATH=%%PATH%%;%s"\n' % os.path.dirname(vswhere))
        f.write('@call "%s\\VC\\Auxiliary\\Build\\vcvarsall.bat" x86 >nul\n' % vs)
        f.write('@set "PATH=%s;%%PATH%%"\n' % llvm)
        wanted = [a for a in sys.argv[1:] if not a.startswith('-')] or list(PROGRAMS)
        for name in wanted:
            sources, libs, run = PROGRAMS[name]
            exe = os.path.join(OUT, name + '.exe')
            files = ' '.join('"%s"' % os.path.join(HERE, s) for s in sources)
            f.write('clang-cl %s /Fo"%s\\\\" %s /Fe:"%s" /link %s\n' % (FLAGS, OUT, files, exe, libs))
            f.write('@if errorlevel 1 exit /b 1\n')
            if run and '--no-run' not in sys.argv:
                f.write('"%s"\n@if errorlevel 1 exit /b 1\n' % exe)
    return subprocess.run(['cmd', '/c', cmd]).returncode


if __name__ == '__main__':
    sys.exit(main())
