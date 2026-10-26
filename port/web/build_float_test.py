import os,subprocess,sys
from pathlib import Path
DEVCACHES = Path(os.environ.get('DHD_DEVCACHES') or Path.home() / 'DevCaches')  # downloaded tools and caches
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'build/web'
env=dict(os.environ,TEMP=str(OUT),TMP=str(OUT))
wasi=str(DEVCACHES / 'wasi-sdk/wasi-sdk-34.0-x86_64-windows/bin/clang.exe')
kind='double' if '--double' in sys.argv else 'float'
src=ROOT/f'port/web/{kind}_fuzz.c'
exports=['web_batch'] if kind=='double' else ['web_fuzz','web_case','web_batch']
builtins=next(Path(wasi).parents[1].glob('lib/clang/*/lib/wasm32-unknown-wasi/libclang_rt.builtins.a'))
subprocess.run([wasi,'-O2','-msimd128','-ffp-contract=off','-nostdlib','-Wl,--no-entry',*['-Wl,--export='+x for x in exports],'-Wl,--initial-memory=134217728',str(src),str(ROOT/f'port/web/{kind}.c'),str(builtins),'-o',str(OUT/f'{kind}-test.wasm')],check=True,env=env)
llvm=(Path.home() / 'scoop/apps/llvm/current/bin')
msvc=Path('C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231')
sdk=Path('C:/Program Files (x86)/Windows Kits/10')
cmd=[str(llvm/'clang-cl.exe'),'/nologo','/O2','/clang:-ffp-model=strict','/clang:-ffp-contract=off','/MT','/imsvc'+str(msvc/'include')]
cmd+=['/imsvc'+str(sdk/'Include/10.0.26100.0'/x) for x in ('ucrt','um','shared')]
cmd += [str(src),'/Fo'+str(OUT/f'{kind}-native.obj'),'/Fe'+str(OUT/f'{kind}-native.exe'),'/link','/libpath:'+str(msvc/'lib/x64')]
cmd+=['/libpath:'+str(sdk/'Lib/10.0.26100.0'/x/'x64') for x in ('ucrt','um')]
subprocess.run(cmd,check=True,env=env)
if kind=='float':
    with (OUT/'float-native.txt').open('w') as log:subprocess.run([str(OUT/'float-native.exe')],stdout=log,check=True,env=env)
