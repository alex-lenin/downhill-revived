"""Fail-closed generator controls and a compile of every host API wrapper.

Requires an earlier build_host64.py --fake build (its exact compiler command
and WABT header are reused). No game data, dependencies or visible windows.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
from gen_host64_imports import NATIVE_ONLY, ROOT, api_declarations, ctype, generate, wasm_type


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--build', type=Path, default=ROOT/'build/host64-exe')
    a = p.parse_args()
    build = a.build.resolve(); out = build/'generator-test'; out.mkdir(exist_ok=True)
    header = (build/'core.h').read_text()
    api = ROOT/'port/host/host_api.h'
    declarations = {name: d for name, d in api_declarations(api).items() if name not in NATIVE_ONLY}
    # Force every API, including posted callbacks, into a WABT-shaped header.
    blocks = []
    for name, (ret, args) in sorted(declarations.items()):
        params = ', '.join(['struct w2c_env*']+[wasm_type(ctype(arg)) for arg in args])
        blocks.append(f"/* import: 'env' '{name}' */\n{wasm_type(ret)} w2c_env_{name}({params});\n")
    all_header = re.sub(r'/\* import:.*?(?=/\* export:)', '', header, flags=re.S)
    point = all_header.index('/* export:')
    all_header = all_header[:point]+'\n'.join(blocks)+'\n'+all_header[point:]
    target = out/'core.h'; target.write_text(all_header)
    rows, _ = generate(target,out)
    assert len(rows)==len(declarations)
    command = next(c for c in json.loads((build/'commands.json').read_text())
                   if '/c' in c and c[c.index('/c')+1].endswith('imports_gen.c'))
    flags = command[:command.index('/c')]
    flags.insert(1,'-I'+str(out))
    result = subprocess.run(flags+['/c',str(out/'imports_gen.c'),'/Fo'+str(out/'all-imports.obj')],
                            cwd=build,capture_output=True,text=True,timeout=120)
    assert result.returncode==0, result.stdout+result.stderr
    print(f'PASS: all {len(rows)} host API wrappers compile')
    cases = {
        'namespace': header.replace("'env' 'host_file_open'", "'other' 'host_file_open'"),
        'unknown': header.replace('host_file_open','unknown_service'),
        'signature': header.replace('u64 w2c_env_host_file_read','u32 w2c_env_host_file_read'),
        'nonfunction': header.replace("/* export: 'memory' */", "/* import: 'env' 'memory' */"),
        'missing-table': header.replace("/* export: '__indirect_function_table' */",'/* hidden table */'),
        'disc-export': all_header.replace("/* export: 'port_disc_validate' */", '/* hidden disc validator */'),
        'disc-signature': all_header.replace('u32 w2c_core_port_disc_validate(w2c_core*, u32, u32, u32);',
                                             'void w2c_core_port_disc_validate(w2c_core*, u32, u32, u32);'),
    }
    for label, text in cases.items():
        case = out/label; case.mkdir(exist_ok=True)
        h = case/'core.h'; h.write_text(text)
        try:
            generate(h,case)
        except ValueError:
            print(f'PASS: generator rejects {label}')
        else:
            raise AssertionError(f'generator accepted {label}')
    # A host C function pointer has the same wasm32 scalar signature as a
    # table index; the native-only services must be refused by name.
    for name in sorted(NATIVE_ONLY):
        case = out/('native-'+name); case.mkdir(exist_ok=True)
        h = case/'core.h'
        h.write_text(header.replace("'env' 'host_callback_fiber'", f"'env' '{name}'"))
        assert h.read_text() != header
        try:
            generate(h,case)
        except ValueError:
            print(f'PASS: generator rejects native-only {name}')
        else:
            raise AssertionError(f'generator accepted native-only {name}')
    # wasi-sdk may export the C main under its argc/argv entry alias.
    alias = header.replace("/* export: 'main' */", "/* export: '__main_argc_argv' */")
    alias = alias.replace('u32 w2c_core_main(w2c_core*);',
                          'u32 w2c_core_0x5F_main_argc_argv(w2c_core*, u32, u32);')
    assert alias != header
    target.write_text(alias)
    generate(target,out)
    result = subprocess.run(flags+['/c',str(out/'imports_gen.c'),'/Fo'+str(out/'alias.obj')],
                            cwd=build,capture_output=True,text=True,timeout=120)
    assert result.returncode==0, result.stdout+result.stderr
    print('PASS: argc/argv entry alias compiles through generated export table')
    # Repeated generation must be byte-for-byte deterministic.
    generate(build/'core.h',out)
    expected = {n:(out/n).read_bytes() for n in ('imports_gen.c','exports_gen.h')}
    generate(build/'core.h',out)
    for name, content in expected.items():
        assert (out/name).read_bytes()==content
    print('PASS: generated boundary is deterministic')


if __name__=='__main__':
    main()
