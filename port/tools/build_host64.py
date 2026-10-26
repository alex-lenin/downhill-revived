"""Build a wasm2c module and the native Windows host as dhd64.exe.

Accepts .wasm or WABT .c units (module name must be core). --fake builds the
independent integration fixture with wasi-sdk. No force-link or missing stubs.
All generated C is prepared before compilation; all output stays in build/.

The GS renderer is the Rust one: port/gs/gpu_rust.c over dhd_render.lib (port/render, built by
port/tools/rustgs.py, release profile) and the libraries rustc lists for it (--print
native-static-libs). The Direct3D 11 and wgpu-native renderers are gone (finding 088, CL2).
"""
import argparse
from concurrent.futures import ThreadPoolExecutor
import json
import os
from pathlib import Path
import subprocess
from gen_host64_imports import generate
from host64_census import HOST_UNITS
import objcache
from prepare_wasm2c import check_launcher_exports, module_units, prepare_units
DEVCACHES = Path(os.environ.get('DHD_DEVCACHES') or Path.home() / 'DevCaches')  # downloaded tools and caches

ROOT = Path(__file__).resolve().parents[2]
WABT = (DEVCACHES / 'wabt/wabt-1.0.42')
WASI = (DEVCACHES / 'wasi-sdk/wasi-sdk-34.0-x86_64-windows')
LLVM = (Path.home() / 'scoop/apps/llvm/current/bin')
MSVC = Path('C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231')
SDK = Path('C:/Program Files (x86)/Windows Kits/10')
VERSION = '10.0.26100.0'
SOURCE = ROOT / 'port/host64'


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('module', type=Path, nargs='*')
    p.add_argument('--fake', action='store_true')
    p.add_argument('--run', action='store_true', help='bounded headless execution')
    p.add_argument('--out', type=Path, default=ROOT / 'build/host64-exe')
    p.add_argument('--sdl-include', type=Path, default=ROOT / 'build/port/sdl3/3.4.18-x86_64/src/SDL3-3.4.18/include')
    p.add_argument('--sdl-lib', type=Path, help='x64 SDL3 static library if module uses controllers')
    p.add_argument('--host-library', type=Path, help='reuse an explicitly supplied x64 host library')
    p.add_argument('--jobs', type=int, default=8)
    p.add_argument('--num-outputs', type=int, default=1, help='WABT split C output count for .wasm input')
    p.add_argument('--wabt', type=Path, default=WABT)
    p.add_argument('--wasi', type=Path, default=WASI)
    p.add_argument('--llvm', type=Path, default=LLVM)
    p.add_argument('--debug', action='store_true',
                   help='line tables and a PDB (for sample_profile.py); code generation is unchanged')
    a = p.parse_args()
    if bool(a.module) == a.fake:
        p.error('provide --fake or one .wasm / a set of wasm2c .c files')
    out = a.out.resolve(); out.mkdir(parents=True, exist_ok=True)
    if out.drive.upper() == 'C:':
        p.error('build output must stay off C:')
    scratch = out / 'tmp'; scratch.mkdir(exist_ok=True)
    env = dict(os.environ, TMP=str(scratch), TEMP=str(scratch), TMPDIR=str(scratch))
    commands = []
    def run(cmd, timeout=180):
        cmd = list(map(str, cmd)); commands.append(cmd)
        r = subprocess.run(cmd, cwd=out, env=env, capture_output=True, text=True,
                           encoding='utf-8', errors='replace', timeout=timeout)
        if r.returncode:
            raise RuntimeError(' '.join(cmd) + '\n' + r.stdout + r.stderr)
        return r.stdout + r.stderr
    rt = a.wabt / 'share/wabt/wasm2c'
    try:
        if a.fake:
            wasm = out / 'fake.wasm'
            run([a.wasi/'bin/clang.exe', '-O2', '-nostdlib', '-ffp-contract=off',
                 '-Wl,--no-entry', '-Wl,--allow-undefined', '-Wl,--export-table',
                 '-Wl,--export-memory', '-Wl,--export=__stack_pointer',
                 '-Wl,--initial-memory=2147483648', '-Wl,--max-memory=2147483648',
                 '-Wl,--stack-first', '-Wl,--global-base=1880096768',  # HOST64_MODULE_BASE (memory_layout.h), as W5
                 SOURCE/'fake_module.c', '-o', wasm])
        elif len(a.module) == 1 and a.module[0].suffix == '.wasm':
            wasm = a.module[0].resolve()
        else:
            wasm = None
        units = module_units(out, run, a.wabt/'bin/wasm2c.exe', wasm, a.module, a.num_outputs)
        rows, exports = generate(out/'core.h', out)
        check_launcher_exports(exports)
        prepare_units(units, out, exact_f32=True)
        # Guard pages (below): WABT keeps every load alive with GNU asm, which it gates on
        # __GNUC__; clang-cl has the same asm but does not define __GNUC__.
        force_read = '#elif defined(__GNUC__) && WASM_RT_MEMCHECK_GUARD_PAGES'
        for source in units + list(out.glob('*-impl.h')):
            text = source.read_text()
            if force_read in text:
                source.write_text(text.replace(force_read, '#elif (defined(__GNUC__) || defined(__clang__)) && '
                                                           'WASM_RT_MEMCHECK_GUARD_PAGES'))
        # The module runs on one OS thread only (its fibers; posted jobs and callbacks are drained
        # there, finding 082 W7e), as its shared __stack_pointer global already requires. WABT's
        # call-depth counter is thread-local, which costs a TLS lookup on every wasm call and
        # return; a copy of wasm-rt.h that lets the build choose the storage class makes it a
        # plain global. The depth check itself (WASM_RT_STACK_DEPTH_COUNT, limit 512) is unchanged.
        header = (a.wabt/'include/wasm-rt.h').read_text()
        block = '#ifdef WASM_RT_C11_AVAILABLE\n#define WASM_RT_THREAD_LOCAL _Thread_local\n'
        if header.count(block) != 1:
            raise ValueError('wasm-rt.h: WASM_RT_THREAD_LOCAL block not found (WABT version changed?)')
        start = header.index(block); end = header.index('#endif\n', start) + len('#endif\n')
        header = (header[:start] + '#ifndef WASM_RT_THREAD_LOCAL\n' + header[start:end] + '#endif\n' +
                  header[end:])
        # Bounds checks by guard pages, as WABT does on every 64-bit GNU host: clang-cl lacks only
        # __GNUC__. guest_memory.c reserves 8 GiB (every wasm32 address plus offset); what lies
        # beyond the memory's size is reserved, never committed, so an out-of-bounds access faults
        # and its reporter names the guest address and ends the process, as the trap did. WABT's
        # own fault-to-trap handler stays off (WASM_RT_SKIP_SIGNAL_RECOVERY): ours handle faults.
        supported = '(WASM_RT_NONCONFORMING_ALLOW_OOB_READ_ELIMINATION || defined(__GNUC__))'
        if header.count(supported) != 1:
            raise ValueError('wasm-rt.h: guard-page support test not found (WABT version changed?)')
        header = header.replace(supported, supported[:-1] + ' || defined(__clang__))')
        (out/'wasm-rt.h').write_text(header)
        common = [a.llvm/'clang-cl.exe', '--target=x86_64-pc-windows-msvc', '/nologo',
                  '/O2', '/clang:-O3', '/clang:-march=x86-64-v2', '/clang:-mtune=znver3',
                  '/clang:-ffp-model=strict', '/clang:-ffp-contract=off',
                  '/clang:-ftrivial-auto-var-init=zero', '/clang:-fno-strict-aliasing',
                  '/clang:-fwrapv', '/Gy', '/Gw', '/MT', '/w',
                  '-DWASM_RT_STACK_DEPTH_COUNT=1', '-DWASM_RT_MAX_CALL_STACK_DEPTH=512',
                  '-DWASM_RT_THREAD_LOCAL=',
                  '-DWASM_RT_MEMCHECK_GUARD_PAGES=1', '-DWASM_RT_SKIP_SIGNAL_RECOVERY=1',
                  '-I'+str(out), '-I'+str(a.wabt/'include'), '-I'+str(rt), '-I'+str(SOURCE),
                  '-I'+str(ROOT/'port/runtime'),
                  '-I'+str(ROOT/'port/include'), '-I'+str(ROOT/'port/host'),
                  '-I' + str(DEVCACHES / 'simde-src'), '/imsvc'+str(MSVC/'include')]
        common += ['/imsvc'+str(SDK/'Include'/VERSION/x) for x in ('ucrt','um','shared')]
        # __FILE__ (and the wide _CRT_WIDE(__FILE__) of assert) would carry this checkout's and the
        # toolchain's folders into dhd64.exe; map them so a packaged exe names no builder path.
        for old, new in ((ROOT, 'src'), (out, 'gen'), (a.wabt, 'wabt'), (a.llvm, 'llvm')):
            for spelling in dict.fromkeys((str(old), str(old).replace('\\', '/'))):  # a set's order varies per run
                common += ['/clang:-ffile-prefix-map=%s=%s' % (spelling, new)]
        if a.debug:
            common += ['/clang:-gline-tables-only', '/clang:-gcodeview']
        if a.fake:
            common += ['-DHOST64_FAKE_MODULE=1']
        else:
            # Traps can come from any EE fiber: report and end (launcher.c).
            common += ['-DWASM_RT_TRAP_HANDLER=host64_trap']
        # Compile every host unit for x64. Archive selection + COMDAT GC keep what the module
        # does not import out of the executable.
        objdir = out/'obj'; objdir.mkdir(exist_ok=True)
        def cached(cmd, obj, timeout):  # the shared object cache (objcache.py); this output folder is a token
            objcache.compile(list(map(str, cmd)), str(obj), lambda c: run(c, timeout=timeout) and None,
                             roots=[(out, 'out')], cwd=out, env=env, hit='')
        # VU1's straight-line blocks as C (vu1_aot.inc and vu1_native_clip.inc, from the game's
        # .vutext: DHD_VUTEXT or assets/vutext.bin); vu0_interp.c runs them
        import vu1_aot
        vu1_aot.generate(out/'vu1_aot.inc')
        def compile_host(rel):
            extra = []
            if rel.endswith('vu0_interp.c'):
                extra += ['-DPS2_VU_AOT']
            if rel.endswith(('gamepad.c', 'present.c', 'sdl_host.c', 'text_input.c', 'sound_out_sdl.c')):
                extra += ['-I'+str(a.sdl_include.resolve())]  # the SDL3 headers (host64_census.SDL_UNITS)
            if rel.endswith('gs/gpu_rust.c'):
                extra += ['-I'+str(ROOT/'port/render/include')]  # dhd_render.h
            if 'libmpeg2/' in rel:
                extra += ['-I'+str(ROOT/'port/third_party/libmpeg2')]
            obj = objdir/(rel.replace('/','_')+'.obj')
            cached(common+extra+['/c', ROOT/rel, '/Fo'+str(obj)], obj, 900)
            return obj
        if a.host_library:
            library = a.host_library.resolve()
            if not library.exists():
                raise ValueError(f'host library does not exist: {library}')
            host_objects = []
        else:
            # gpu_rust.c is in HOST_UNITS; the WGSL is in dhd_render.lib; gpu_rust_test.c is for
            # test builds only
            with ThreadPoolExecutor(a.jobs) as pool:
                host_objects = list(pool.map(compile_host, HOST_UNITS))
            library = out/'host64.lib'
            run([a.llvm/'llvm-lib.exe', '/out:'+str(library), *host_objects])
        sources = units + [rt/'wasm-rt-impl.c', out/'imports_gen.c'] + [SOURCE/n for n in
                ('guest_memory.c','guest_memory_win32.c','fiber_x64.c','fp_mode.c','bridge.c','diagnostics.c','launcher.c')]
        objects = [objdir/(source.stem+'.obj') for source in sources]
        with ThreadPoolExecutor(a.jobs) as pool:
            list(pool.map(lambda so: cached(common+['/c',so[0],'/Fo'+str(so[1])], so[1], 1800),
                          zip(sources, objects)))
        # dxgi.lib: gpu_rust.c's adapter choice and video memory (CreateDXGIFactory1)
        libs = ['onecore.lib','user32.lib','gdi32.lib',
                'dxgi.lib','winhttp.lib','ole32.lib','shell32.lib','advapi32.lib',
                'winmm.lib','ws2_32.lib','comdlg32.lib','uuid.lib']
        if a.sdl_lib:
            from sdl3 import SYSTEM_LIBS
            libs += [str(a.sdl_lib.resolve())] + [x for x in SYSTEM_LIBS if x not in libs]
        import rustgs  # cargo through rustgs only (its flags, version gate, checks and cache)
        try:
            report = rustgs.build(rustgs.X64)
        except rustgs.Refused as why:
            raise RuntimeError('rustgs: refused: %s' % why)
        # rustc's list (checked against S1's by rustgs): import libs and /defaultlib:libcmt,
        # which /MT already names; raw-dylib imports (d3d12, dxgi, ...) need no lib. dxguid.lib:
        # gpu_rust.c's IID_IDXGIFactory1 and IID_IDXGIDevice
        libs += [report['lib']] + [x for x in report['native_static_libs'] + ['dxguid.lib'] if x not in libs]
        rsp = out/'link.rsp'
        rsp.write_text('\n'.join('"'+str(x)+'"' for x in objects+[library]+libs))
        link = [a.llvm/'lld-link.exe', '@'+str(rsp), '/out:'+str(out/'dhd64.exe'),
                '/libpath:'+str(MSVC/'lib/x64'),
                '/libpath:'+str(SDK/'Lib'/VERSION/'ucrt/x64'),
                '/libpath:'+str(SDK/'Lib'/VERSION/'um/x64'),
                '/subsystem:windows','/entry:mainCRTStartup','/opt:ref','/opt:icf','/dynamicbase','/nxcompat',  # windowed: no console on a double-click, the log goes to dhd.log (host_console_init)
                '/map:'+str(out/'dhd64.map'), '/errorlimit:0']
        if a.debug:
            link += ['/debug', '/pdb:'+str(out/'dhd64.pdb')]
        link_output = run(link)
        (out/'link.log').write_text(link_output)
        (out/'failure.log').unlink(missing_ok=True)
        host_summary = 'reused x64 host library' if a.host_library else f'{len(host_objects)} x64 host objects'
        print(f'PASS: {len(rows)} generated imports; {host_summary}; {out / "dhd64.exe"}', flush=True)
        if a.run:
            if a.fake:
                (out/'fixture.txt').write_bytes(b'host64-proof')
            env.update(DHD_HEADLESS='1', DHD_SND_OFF='1')
            result = run([out/'dhd64.exe'], timeout=60)
            if a.fake:
                for test in range(1,12):
                    result += run([out/'dhd64.exe','--negative',test],timeout=30)
            (out/'run.log').write_text(result)
            print(result, end='')
    except (RuntimeError, ValueError) as error:
        (out/'failure.log').write_text(str(error))
        raise SystemExit(str(error))
    finally:
        (out/'commands.json').write_text(json.dumps(commands, indent=2)+'\n')


if __name__ == '__main__':
    main()
