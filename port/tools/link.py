"""PC build, link stage and link census: link the objects of port/tools/build.py, the asm runtime
and the data of port/tools/data.py into build/port/link/dhd.exe (32-bit Windows, lld-link with
the MSVC C runtime; port/tools/pctarget.py) and sort what doesn't resolve.

C++ objects get their functions' and globals' retail names back first: a C++ function is
emitted under a mangled name (_Z13func_00123456P3Obj) that depends on the declaration a unit
happened to see, while the data, the C units and the PS2 itself know it as func_00123456. Every
symbol whose demangled identifier is a retail name (func_XXXXXXXX, D_XXXXXXXX) is renamed to
that name (with the target's C prefix), in copies under build/port/link/obj/.

Game, engine and crt0 functions that are still asm only are recompiled to C (port/tools/recomp.py)
and linked in, together with whatever further unfinished functions those call.

The census lists undefined symbols by kind (asm-only functions, SDK/library, other) and
duplicate definitions; build/port/link/census.json keeps the full lists. Unresolved symbols are
forced through (/force), so dhd.exe is written but not runnable yet.
usage: python port/tools/link.py   (needs LLVM's clang, lld-link, llvm-nm, llvm-objcopy, llvm-rc,
llvm-cxxfilt on PATH and Visual Studio's x86 libraries; run port/tools/build.py first)"""
import collections, glob, json, os, re, shutil, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import data, recomp, sdl3  # noqa: E402
from pctarget import HOST_TARGET, IMAGE_BASE, LABEL_PREFIX as P, TARGET  # noqa: E402

ROOT = data.ROOT
OBJ = os.path.join(ROOT, 'build', 'port', 'obj')
OUT = os.path.join(ROOT, 'build', 'port', 'link')
RETAIL = re.compile(r'^(?:func|D)_[0-9A-F]{8}$')
VARIANT = re.compile(r'\d(?:func|D)_[0-9A-F]{8}(?:C2|D2|D0)E')  # base ctor/dtor, deleting dtor


def unprefixed(sym):
    return sym[len(P):] if sym.startswith(P) else sym


def run(cmd, **kw):
    # which() so that a name like 'ld.lld' finds the .exe (CreateProcess won't add it there)
    cmd = [shutil.which(cmd[0]) or cmd[0]] + cmd[1:]
    return subprocess.run(cmd, capture_output=True, text=True, encoding='utf-8', errors='replace', **kw)


def nm(paths, *flags):
    """{path: [symbol, ...]} via llvm-nm, in batches."""
    res = collections.defaultdict(list)
    for i in range(0, len(paths), 200):
        batch = paths[i:i + 200]
        cur = batch[0] if len(batch) == 1 else None
        for line in run(['llvm-nm', '-f', 'just-symbols', *flags] + batch).stdout.splitlines():
            if line.endswith(':') and line[:-1] in batch:
                cur = line[:-1]
            elif line and cur:
                res[cur].append(line)
    return res


def retail_name(demangled):
    """func_00123456 for 'ns::Obj::func_00123456(Obj*, int)', or None."""
    base = demangled.split('(')[0].split('::')[-1].strip()
    return base if RETAIL.match(base) else None


def rename_cpp(objs):
    cpp = [o for o in objs if o.endswith('.cpp.o')]
    # A class named after its address (class func_00166F28) gets two constructors here, complete
    # (C1) and base (C2), and up to three destructors; gcc 2.95 had one of each, the retail name
    # goes to the complete one. The other variants keep their mangled names where a unit defines
    # them; a variant that is only called (a base class defined in C or asm) gets the retail name.
    defined = {s for v in nm(cpp, '--defined-only').values() for s in v}
    syms = sorted({s for v in nm(cpp).values() for s in v
                   if unprefixed(s).startswith('_Z') and not (VARIANT.search(s) and s in defined)})
    dem = run(['llvm-cxxfilt'], input=''.join(unprefixed(s) + '\n' for s in syms)).stdout.splitlines()
    mapping = {m: P + retail_name(d) for m, d in zip(syms, dem) if retail_name(d)}
    os.makedirs(OUT, exist_ok=True)
    mapfile = os.path.join(OUT, 'retail_names.txt')
    with open(mapfile, 'w', newline='\n') as f:
        f.writelines('%s %s\n' % kv for kv in sorted(mapping.items()))

    def one(o):
        dst = os.path.join(OUT, 'obj', os.path.relpath(o, OBJ))
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        p = run(['llvm-objcopy', '--redefine-syms=' + mapfile, o, dst])
        if p.returncode:
            raise RuntimeError(p.stderr)
        return dst
    with ThreadPoolExecutor(8) as ex:
        renamed = dict(zip(cpp, ex.map(one, cpp)))
    return [renamed.get(o, o) for o in objs], len(mapping)


ASM_ADDR = re.compile(r'/\* [0-9A-F]+ ([0-9A-F]{8}) ')
REGIONS = [  # (start, end, name), as in tools/queue.py
    (0x10A000, 0x10A208, 'crt0'), (0x10A208, 0x1FC000, 'game'), (0x1FC000, 0x201000, 'libgcc'),
    (0x201000, 0x243000, 'engine'), (0x243000, 0x263BC0, 'sdk')]
LIBC = {'abs', 'memcpy', 'memmove', 'memset', 'memcmp', 'strcpy', 'strncpy', 'strcat', 'strncat',
        'strcmp', 'strncmp', 'strlen', 'memchr', 'strchr', 'strrchr', 'strstr', 'sprintf', 'vsprintf', 'printf',
        'fabsf', 'floorf', 'ceilf', 'sqrtf', 'sinf', 'cosf', 'atan2f', 'powf', 'malloc', 'free', 'rand',
        'srand', 'atoi', 'toupper', 'tolower'}


def region(addr):
    return next((n for a, b, n in REGIONS if a <= addr < b), None)


def asm_functions():
    """{name: vram} of every function splat wrote a .s for (decompiled or not)."""
    out = {}
    for p in glob.glob(os.path.join(ROOT, 'asm', 'nonmatchings', '**', '*.s'), recursive=True):
        m = ASM_ADDR.search(open(p, encoding='utf-8', errors='replace').read(4096))
        if m:
            out[os.path.splitext(os.path.basename(p))[0]] = int(m.group(1), 16)
    return out


def library_functions():
    """{name: archive} from config/libmatch.txt (ADDR NAME ARCHIVE:MEMBER WORDS)."""
    out = {}
    for line in open(os.path.join(ROOT, 'config', 'libmatch.txt')):
        parts = line.split()
        if len(parts) > 2 and not line.startswith('#'):
            out[parts[1]] = parts[2].split(':')[0]
    return out


def kind(name, asm, lib):
    if lib.get(name) in ('libc.a', 'libm.a', 'libgcc.a') or name in LIBC:
        return 'C library / libgcc (the host provides them)'
    if name in RECOMP_FAILED:
        return 'SDK function to write natively (port/hle): %s' % RECOMP_FAILED[name].split(': ', 1)[-1][:40]
    if name in lib:
        return 'SDK library function (%s)' % lib[name]
    if name.startswith('__builtin'):
        return 'gcc 2.95 builtin'
    addr = asm.get(name)
    if addr is None and RETAIL.match(name):
        addr = int(name[-8:], 16)
    reg = region(addr) if addr is not None else None
    if name in asm and reg in ('game', 'engine', 'crt0'):
        return 'asm-only %s function (not decompiled: queue, hand-written, remnant)' % reg
    if name in asm and reg:
        return 'asm-only %s function' % reg
    if reg:
        return 'code address used as data (entry point inside an asm %s function)' % reg
    if name == 'main':
        return 'PC entry point (written with the boot code)'
    if name.startswith('_Z'):
        return 'C++ name with no retail id'
    return 'other'


def runtime():
    """The inline asm runtime (port/runtime) and the native PS2 system (port/hle): host C, built
    against the platform's headers."""
    out = []
    aot = os.environ.get('DHD_BUILD_VU_AOT') == '1'
    if aot:
        from vu1_aot import generate
        print('VU1 AOT: %d straight-line blocks' % generate(os.path.join(OUT, 'vu1_aot.inc')))
    shader = open(os.path.join(ROOT, 'port', 'gs', 'raster.hlsl'), encoding='utf-8').read()
    with open(os.path.join(OUT, 'gs_shader.h'), 'w', encoding='utf-8') as f:
        f.write('static const char gs_shader[] = ' + json.dumps(shader) + ';\n')
        # A repo-relative name (shader error messages only), so no builder path ends up in dhd.exe.
        f.write('static const char gs_shader_path[] = ' + json.dumps('port/gs/raster.hlsl') + ';\n')
    for name in ('runtime/ps2asm.c', 'runtime/vu0_micro.c', 'runtime/vu0_interp.c', 'runtime/recomp.c', 'runtime/fp_trap.c', 'runtime/libc_shims.c',
                 'runtime/memory.c', 'hle/host.c', 'hle/autopad.c', 'hle/timer.c', 'hle/kernel.c', 'hle/boot.c', 'hle/sif.c', 'hle/cdvd.c', 'hle/game_data.c',
                 'hle/iop.c', 'hle/mc.c', 'hle/dma.c', 'hle/vif.c', 'hle/graphics.c', 'hle/graphics_queue.c', 'hle/pad.c', 'hle/menu_page.c', 'hle/pc_settings.c', 'hle/widescreen.c', 'hle/exit_menu.c', 'hle/online_menu.c', 'hle/online.c', 'hle/puppet.c', 'hle/sound.c', 'gs/memory.c', 'gs/gs.c', 'gs/display.c', 'gs/present.c', 'gs/gpu.c', 'gs/texpack.c', 'gs/font_sharp.c', 'gs/capture.c',
                 # the auto-updater (finding 054)
                 'hle/update.c', 'hle/game_options.c', 'hle/fps.c', 'hle/splash.c',
                 # the game's movies (finding 055)
                 'hle/movie.c',
                 # online races (port/net, finding 042)
                 'net/lockstep.c', 'net/hash.c', 'net/json.c', 'net/client.c', 'net/ws_winhttp.c', 'net/net.c',
                 # the keyboard and the controllers through SDL3 (finding 059)
                 'hle/keyboard.c', 'hle/text_input.c', 'hle/gamepad.c'):
        o = os.path.join(OUT, name + '.o')
        os.makedirs(os.path.dirname(o), exist_ok=True)
        extra = ['-DPS2_VU_AOT'] if aot and name == 'runtime/vu0_interp.c' else []
        if name == 'hle/gamepad.c':
            extra = ['-I', str(sdl3.build()[1])]
        p = run(['clang', '--target=' + HOST_TARGET, '-c', '-O2', '-msse2', '-mfpmath=sse',
                 '-ffp-contract=off', '-w', '-I', OUT, '-I', os.path.join(ROOT, 'port', 'runtime'),
                 os.path.join(ROOT, 'port', name), '-o', o] + extra)
        if p.returncode:
            sys.exit(p.stderr)
        out.append(o)
    # The movies' MPEG-2 video decoder: libmpeg2's portable C core (port/third_party/libmpeg2,
    # GPL-2-or-later, README.port there), with its own config.h.
    lib = os.path.join(ROOT, 'port', 'third_party', 'libmpeg2')
    for name in ('alloc', 'cpu_accel', 'cpu_state', 'decode', 'header', 'idct', 'motion_comp', 'slice'):
        o = os.path.join(OUT, 'libmpeg2', name + '.c.o')
        os.makedirs(os.path.dirname(o), exist_ok=True)
        p = run(['clang', '--target=' + HOST_TARGET, '-c', '-O2', '-msse2', '-mfpmath=sse', '-w', '-I', lib,
                 os.path.join(lib, name + '.c'), '-o', o])
        if p.returncode:
            sys.exit(p.stderr)
        out.append(o)
    return out


HOST_CFLAGS = ['--target=' + HOST_TARGET, '-c', '-O1', '-w', '-fno-strict-aliasing', '-fwrapv', '-msse2',
               '-mfpmath=sse', '-ffp-contract=off', '-I', os.path.join(ROOT, 'port', 'runtime'),
               '-I', os.path.join(ROOT, 'port', 'include')]


RECOMP_FAILED = {}  # {SDK function: why the recompiler couldn't translate it}


def recompiled(objs):
    """Objects of the recompiled asm-only functions the link needs, to a fixed point."""
    asm = asm_functions()
    in_data = set()  # function pointers in the data (vtables, callback tables)
    for p in glob.glob(os.path.join(ROOT, 'asm', 'data', '*.s')):
        in_data |= set(re.findall(r'\bfunc_[0-9A-F]{8}\b', open(p, encoding='utf-8').read()))
    in_data |= set(recomp.data_entries())  # entries the data holds as numbers
    asm.update({n: e for n, (_, e) in recomp.data_entries().items()})
    names, out = set(), []
    while True:
        defined = {unprefixed(s) for v in nm(objs + out, '--defined-only', '-g').values() for s in v}
        # port/hle may call a recompiled function at its register entry, rc_<name>
        wanted = in_data | {unprefixed(s).removeprefix('rc_') for v in nm(objs + out, '--undefined-only').values()
                            for s in v}
        new = sorted(n for n in wanted - defined - names - LIBC  # the host's C library has those
                     if n in asm and region(asm[n]) in ('game', 'engine', 'crt0', 'libgcc', 'sdk'))
        if not new:
            return out, sorted(names)
        names |= set(new)
        paths, failures = recomp.recompile(sorted(names))
        game = {n: e for n, e in failures.items() if region(asm[n]) != 'sdk'}
        if game:
            sys.exit('recompilation failed: %r' % game)
        # an SDK function the recompiler can't translate (syscall, COP0) needs a native one (port/hle)
        RECOMP_FAILED.update(failures)

        def one(c):
            o = c + '.o'
            p = run(['clang'] + HOST_CFLAGS + [c, '-o', o])
            if p.returncode:
                raise RuntimeError(p.stderr)
            return o
        with ThreadPoolExecutor(8) as ex:
            out = list(ex.map(one, paths))


def main():
    objs = sorted(glob.glob(os.path.join(OBJ, '**', '*.o'), recursive=True))
    if not objs:
        sys.exit('no objects in build/port/obj: run port/tools/build.py first')
    objs, nrenamed = rename_cpp(objs)
    objs += runtime()
    robjs, rnames = recompiled(objs)
    objs += robjs
    image = data.ImageNames()
    clash = sorted(s for v in nm(objs, '--defined-only', '-g').values() for s in v if unprefixed(s) in image)
    if clash:  # transform.extern_image_data missed a definition: two copies of one global
        sys.exit('defined in C and in the PS2 data image: %s' % ', '.join(clash[:20]))
    wanted = {s for v in nm(objs, '--undefined-only').values() for s in v}
    sources, labels, fixups = data.convert_all(wanted)

    def assemble(s):
        p = run(['clang', '--target=' + TARGET, '-c', s, '-o', s + '.o'])
        if p.returncode:
            sys.exit(p.stderr)
        return s + '.o'
    image_obj, symbols_obj = (assemble(s) for s in sources)
    image_nm = run(['llvm-nm', image_obj]).stdout.splitlines()
    bad = data.check_image(image_obj, labels, image_nm)
    if bad:
        sys.exit('PS2 data image misplaced: %s' % '; '.join(bad[:10]))
    # The image's bytes stay out of dhd.exe: they come from the player's disc at start
    # (port/hle/game_data.c). dhd.exe holds only the words that differ from them.
    try:
        fixups_s, nfix = data.write_fixups(image_obj, fixups, image_nm)
    except (OSError, ValueError) as e:
        sys.exit('PS2 data image: %s' % e)
    # the icon (port/res/dhd.rc), shown by Explorer, the taskbar and the game's windows
    res = os.path.join(OUT, 'dhd.res')
    p = run(['llvm-rc', '/fo', res, os.path.join(ROOT, 'port', 'res', 'dhd.rc')])
    if p.returncode:
        sys.exit(p.stderr)
    dobjs = [symbols_obj, assemble(fixups_s), res]
    # SDL3, static (port/tools/sdl3.py), and the Windows libraries it needs: in the object list,
    # so build_clean.py's relink gets them too
    libs = [str(sdl3.build()[0])] + ['-Wl,' + name for name in sdl3.SYSTEM_LIBS]
    rsp = os.path.join(OUT, 'objs.rsp')
    with open(rsp, 'w', newline='\n') as f:
        f.write(''.join(o.replace(os.sep, '/') + '\n' for o in objs + dobjs + libs))
    def link(extra=()):
        return run(['clang', '--target=' + HOST_TARGET, '-fuse-ld=lld', '-o', os.path.join(OUT, 'dhd.exe'),
                    '@' + rsp, '-Wl,d3dcompiler.lib', '-Wl,d3d11.lib', '-Wl,dxgi.lib', '-Wl,user32.lib', '-Wl,gdi32.lib', '-Wl,winhttp.lib', '-Wl,ole32.lib', *extra, '-Wl,/map:' + os.path.join(OUT, 'dhd.map'), '-Wl,/subsystem:windows', '-Wl,/entry:mainCRTStartup', '-Wl,/safeseh:no', '-Wl,/demangle:no',
                    '-Wl,/force', '-Wl,/errorlimit:0',
                    # above the emulated PS2 RAM and its mirrors (port/runtime/memory.c), never moved
                    '-Wl,/base:0x%X' % IMAGE_BASE, '-Wl,/fixed', '-Wl,/dynamicbase:no',
                    # the PS2 never protected .rodata and the game writes some (func_00208900
                    # uppercases a const bank name in place)
                    '-Xlinker', '/section:.rdata,RW'])
    p = link()
    if p.returncode:
        sys.exit(p.stderr)

    undefined = collections.defaultdict(set)
    for name, refs in re.findall(r'undefined symbol: (.+)\n((?:>>> .*\n?)*)', p.stderr):
        undefined[unprefixed(name)] |= set(re.findall(r'>>> referenced by .*?\(([^)]+)\)', refs))
    duplicates = {}
    for name, defs in re.findall(r'duplicate symbol: (\S+)(.*\n(?:>>> .*\n?)*)', p.stderr):
        duplicates[unprefixed(name)] = re.findall(r'([^\\/ ]+\.o)', defs)
    asm, lib = asm_functions(), library_functions()
    kinds = collections.defaultdict(list)
    for name in sorted(undefined):
        kinds[kind(name, asm, lib)].append(name)
    json.dump({'undefined': {k: sorted(undefined[k]) for k in sorted(undefined)},
               'kinds': kinds, 'duplicates': duplicates},
              open(os.path.join(OUT, 'census.json'), 'w'), indent=1)
    other = [l for l in p.stderr.splitlines() if ('error:' in l or 'warning:' in l)
             and 'undefined symbol' not in l and 'duplicate symbol' not in l]
    print('%d objects + PS2 data image fixups (%d labels placed as on the PS2, %d words differ from the disc); '
          '%d C++ symbols renamed to retail names; %d asm-only functions recompiled' % (
              len(objs), len(labels), nfix, nrenamed, len(rnames)))
    print('%d undefined symbols, %d duplicate definitions, %d other errors/warnings' % (
        len(undefined), len(duplicates), len(other)))
    for k, v in sorted(kinds.items(), key=lambda kv: -len(kv[1])):
        print('%6d  %s   e.g. %s' % (len(v), k, ', '.join(v[:4])))
    for l in other[:10]:
        print(l)
    stubs = [n for n in sorted(undefined) if not n.startswith(('D_', '.L', 'rc_'))]
    if stubs:  # link again with a stub for each missing function, which names it when called
        c = os.path.join(OUT, 'missing.c')
        with open(c, 'w', newline='\n') as f:
            f.write('/* Generated by port/tools/link.py: the functions the link is missing. */\n'
                    'void hle_missing(const char *name);\n')
            f.write(''.join('void %s(void) { hle_missing("%s"); }\n' % (n, n) for n in stubs if n != 'main'))
        p = run(['clang'] + HOST_CFLAGS + [c, '-o', c + '.o'])
        if p.returncode:
            sys.exit(p.stderr)
        p = link([c.replace(os.sep, '/') + '.o'])
        if p.returncode:
            sys.exit(p.stderr)
        left = sorted(set(re.findall(r'undefined symbol: (.+)', p.stderr)))
        print('dhd.exe linked with %d stubs; still undefined: %s' % (len(stubs), ', '.join(left) or 'none'))


if __name__ == '__main__':
    main()
