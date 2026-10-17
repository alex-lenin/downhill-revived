"""The matching toolchain: one definition shared by the grader and the build.

Game code = SN ProDG 3.01: ee-gcc2953.exe (cc1 2.95.3 SN BUILD 1.36) to assembly, then SN's
ps2eeas. Flags in CFLAGS. Install with tools/setup_toolchain.py.
"""
import os, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SN = os.path.join(ROOT, 'tools', 'compilers', 'sn-prodg-3.01', 'usr', 'local', 'sce', 'ee', 'gcc')

CC = os.path.join(SN, 'bin', 'ee-gcc2953.exe')       # driver; runs cpp/cc1/cc1plus
PS2EEAS = os.path.join(SN, 'ee', 'bin', 'Ps2EeAs.exe')  # SN assembler: compiled code
AS = os.path.join(SN, 'bin', 'ee-as.exe')             # GNU as 2.9-ee: hand/extracted asm
LD = os.path.join(SN, 'bin', 'ee-ld.exe')
OBJCOPY = os.path.join(SN, 'bin', 'ee-objcopy.exe')
OBJDUMP = os.path.join(SN, 'bin', 'ee-objdump.exe')

# The toolchain is Windows binaries. Elsewhere (Linux, WSL, cloud VMs) they run under Wine,
# which compiles byte-identically (checked against the Windows build's objects).
WINE = os.name != 'nt'
if WINE:
    os.environ.setdefault('WINEDEBUG', '-all')
    # Many parallel wine clients race an exiting wineserver ("wine client error ... Connection
    # reset by peer"); a persistent server avoids that. Cheap when one is already running.
    # Distros differ on where wineserver lives (Ubuntu: /usr/lib/<triplet>/wine/, not PATH).
    import glob, shutil
    _ws = (shutil.which('wineserver') or shutil.which('wineserver-stable')
           or next(iter(glob.glob('/usr/lib/*/wine/wineserver') + glob.glob('/usr/lib/wine/wineserver*')),
                   None))
    if _ws:
        subprocess.run([_ws, '-p'], stdin=subprocess.DEVNULL,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def exe(path):
    """Command prefix that runs the Windows tool PATH on this host."""
    return ['wine', path] if WINE and path.lower().endswith('.exe') else [path]


CFLAGS = ['-O2', '-G8', '-fopt-stack']
# Retail .data holds only 4 DWARF CIEs, i.e. only 4 C++ TUs were built with exceptions (frame
# info); g++ 2.95 emits it for every function by default. Units that need it add -fexceptions.
CXXFLAGS = CFLAGS + ['-fno-exceptions']
ASFLAGS_PS2EEAS = ['-G8']


def check():
    for p in (CC, PS2EEAS, AS, LD, OBJCOPY):
        if not os.path.exists(p):
            sys.exit('toolchain missing (%s); run: python tools/setup_toolchain.py' % p)


def run(cmd):
    for _ in range(3):  # under Wine, retry a lost connection to the wineserver
        p = subprocess.run(exe(cmd[0]) + cmd[1:], capture_output=True, text=True,
                           stdin=subprocess.DEVNULL)
        if not (WINE and p.returncode and 'wine client error' in p.stderr):
            break
    if p.returncode:
        sys.stderr.write(p.stdout + p.stderr)
    return p.returncode == 0


def compile_c(src, out_o, lang=None, flags=None, include=()):
    """C/C++ -> .s (SN cc1) -> .o (ps2eeas). Returns True on success."""
    check()
    flags = CFLAGS if flags is None else flags
    s = out_o[:-2] + '.s'
    lang = lang or ('c' if src.endswith('.c') else 'c++')
    inc = [x for d in include for x in ('-I', d)]
    if not run([CC, '-x', lang, '-S', src, '-o', s] + inc + flags):
        return False
    g = [f for f in flags if f.startswith('-G')][-1:] or ASFLAGS_PS2EEAS
    return run([PS2EEAS] + g + ['-o', out_o, s])
