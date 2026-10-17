"""Snapshot an existing native link's COFF inputs and relink comparison hooks privately.

Avoids regenerating shared recomp/data files while other workers relink. Outputs
contain proprietary game data and must stay under ignored build/.
"""
from pathlib import Path
import argparse
import hashlib
import json
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/compare-build'
OWN = ('hle/host.c', 'hle/autopad.c', 'gs/present.c')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--link', type=Path, default=ROOT / 'build/port/link',
                        help='link directory holding objs.rsp (another worker may link elsewhere)')
    parser.add_argument('--rebuild', nargs='*', default=[], metavar='FILE',
                        help='more port/ host sources to rebuild from the tree, e.g. runtime/ps2asm.c')
    parser.add_argument('--recomp', nargs='*', default=[], metavar='FUNC',
                        help="recompiled functions to rebuild from the link's recomp/ C with the tree's runtime "
                             'headers, e.g. func_0025AB70')
    parser.add_argument('--unit', nargs='*', default=[], metavar='UNIT',
                        help='src/ units to recompile with port/patches.py, e.g. unsorted/0016B8C8.cpp')
    parser.add_argument('--runtime-isa', choices=('sse2', 'sse4.1', 'avx2'), default='sse2',
                        help='experimental runtime ISA; contraction stays disabled')
    parser.add_argument('--runtime-thinlto', action='store_true', help='ThinLTO for rebuilt runtime objects')
    parser.add_argument('--game-objects', type=Path, help='replace all linked game objects from a private build')
    parser.add_argument('--recomp-opt', choices=('1', '2'), default='1')
    parser.add_argument('--recomp-all', action='store_true', help='rebuild every frozen recompiled function')
    parser.add_argument('--pgo-use', type=Path, help='instrumentation profile for the VU runtime')
    args = parser.parse_args()
    link = args.link.resolve()
    own = OWN + tuple(args.rebuild)
    sources = [Path(line.strip().strip('"')) for line in (link / 'objs.rsp').read_text().splitlines() if line.strip()]
    sources.append(link / 'missing.c.o')
    OUT.mkdir(exist_ok=True)
    objects = OUT / 'objects'
    objects.mkdir(exist_ok=True)
    # The tree's shader, as port/tools/link.py embeds it (only rebuilt gs/gpu.c objects use it).
    shader = (ROOT / 'port/gs/raster.hlsl').read_text(encoding='utf-8')
    (OUT / 'gs_shader.h').write_text('static const char gs_shader[] = ' + json.dumps(shader) + ';\n' +
                                     'static const char gs_shader_path[] = ' +
                                     json.dumps(str(ROOT / 'port/gs/raster.hlsl')) + ';\n', encoding='utf-8')
    copied = []
    hashes = hashlib.sha256()
    for i, source in enumerate(sources):
        target = objects / f'{i:05d}-{source.name}'
        for attempt in range(4):
            try:
                before = source.stat()
                content = source.read_bytes()
                after = source.stat()
                if before.st_mtime_ns != after.st_mtime_ns or before.st_size != len(content):
                    raise OSError('object changed during read')
                target.write_bytes(content)
                break
            except OSError:
                if attempt == 3:
                    raise
                time.sleep(0.1)
        hashes.update(hashlib.sha256(content).digest())
        copied.append(target)
    for name in own:
        source_object = link / (name + '.o')
        i = sources.index(source_object)
        target = copied[i]
        extra = ['-flto=thin'] if args.runtime_thinlto else []
        if args.pgo_use and name == 'runtime/vu0_interp.c':
            extra += ['-fprofile-instr-use=' + str(args.pgo_use.resolve()), '-Werror=profile-instr-out-of-date']
        if name == 'runtime/vu0_interp.c' and (link / 'vu1_aot.inc').exists():
            # The link was built with DHD_BUILD_VU_AOT=1: translate VU1 again from the tree, as link.py does.
            import sys
            sys.path.insert(0, str(ROOT / 'port/tools'))
            from vu1_aot import generate
            generate(OUT / 'vu1_aot.inc')
            extra += ['-DPS2_VU_AOT']
        subprocess.run(['clang', '--target=i686-pc-windows-msvc', '-c', '-O2', '-m' + args.runtime_isa, '-mno-fma',
                        '-mfpmath=sse', '-ffp-contract=off', '-w', '-I', str(OUT),
                        '-I', str(ROOT / 'port/runtime'), str(ROOT / 'port' / name), '-o', str(target)] + extra,
                       check=True)
    if args.recomp_all:
        args.recomp = [p.name[:-4] for p in sources if p.parent == link.parent / 'recomp']
    for name in args.recomp:
        # Same flags as port/tools/link.py's HOST_CFLAGS for recompiled functions.
        source = link.parent / 'recomp' / (name + '.c')
        target = copied[sources.index(Path(str(source) + '.o'))]
        subprocess.run(['clang', '--target=i686-pc-windows-msvc', '-c', '-O' + args.recomp_opt, '-w', '-fno-strict-aliasing', '-fwrapv',
                        '-msse2', '-mfpmath=sse', '-ffp-contract=off', '-I', str(ROOT / 'port/runtime'), str(source),
                        '-o', str(target)], check=True)
    for unit in args.unit:
        # port/tools/build.py writes build/port/obj; C++ units then take the link's retail names.
        subprocess.run(['python', str(ROOT / 'port/tools/build.py'), str(ROOT / 'src' / unit)], check=True)
        built = ROOT / 'build/port/obj' / (unit + '.o')
        source_object = (link / 'obj' if unit.endswith('.cpp') else link.parent / 'obj') / (unit + '.o')
        if source_object not in sources:
            raise RuntimeError(f'{unit} is not a separately linked unit in {link}')
        target = copied[sources.index(source_object)]
        if unit.endswith('.cpp'):
            subprocess.run(['llvm-objcopy', '--redefine-syms=' + str(link / 'retail_names.txt'), str(built), str(target)],
                           check=True)
        else:
            shutil.copyfile(built, target)
    if args.game_objects:
        for i, source in enumerate(sources):
            if source.parent.parent not in (link / 'obj', link.parent / 'obj'):
                continue
            unit = source.relative_to(source.parent.parent)
            built = args.game_objects.resolve() / unit
            if not built.is_file():
                raise RuntimeError(f'missing replacement object: {built}')
            if str(unit).endswith('.cpp.o'):
                subprocess.run(['llvm-objcopy', '--redefine-syms=' + str(link / 'retail_names.txt'), str(built), str(copied[i])], check=True)
            else:
                shutil.copyfile(built, copied[i])
    rsp = OUT / 'objects.rsp' 
    rsp.write_text(''.join('"' + path.as_posix() + '"\n' for path in copied))
    exe = OUT / 'dhd.exe'
    subprocess.run(['clang', '--target=i686-pc-windows-msvc', '-fuse-ld=lld', '-o', str(exe), '@'+str(rsp),
                    '-Wl,d3dcompiler.lib', '-Wl,d3d11.lib', '-Wl,dxgi.lib', '-Wl,user32.lib', '-Wl,ole32.lib',
                    '-Wl,/threads:6', '-Wl,/opt:lldltojobs=6', '-Wl,/map:'+str(exe.with_suffix('.map')), '-Wl,/subsystem:console', '-Wl,/safeseh:no',
                    '-Wl,/demangle:no', '-Wl,/base:0x40000000', '-Wl,/fixed', '-Wl,/dynamicbase:no',
                    '-Xlinker', '/section:.rdata,RW'], check=True)
    (OUT / 'snapshot.json').write_text(json.dumps(dict(objects=len(copied), source_object_digest=hashes.hexdigest(),
                                                      rebuilt=list(own) + list(args.recomp) + list(args.unit),
                                                      runtime_isa=args.runtime_isa, runtime_thinlto=args.runtime_thinlto, recomp_opt=args.recomp_opt,
                                                      game_objects=str(args.game_objects) if args.game_objects else None,
                                                      pgo_sha256=hashlib.sha256(args.pgo_use.read_bytes()).hexdigest() if args.pgo_use else None), indent=2))
    print(exe)


if __name__ == '__main__':
    main()
