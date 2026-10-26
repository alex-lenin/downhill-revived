import os,subprocess
from pathlib import Path
from postlink import rewrite,Module,exports,uleb,enc_u,instructions
DEVCACHES = Path(os.environ.get('DHD_DEVCACHES') or Path.home() / 'DevCaches')  # downloaded tools and caches
ROOT=Path(__file__).resolve().parents[2];out=ROOT/'build/web'
subprocess.run([str(DEVCACHES / 'wasi-sdk/wasi-sdk-34.0-x86_64-windows/bin/clang.exe'),'-O2','-msimd128','-nostdlib','-Wl,--no-entry','-Wl,--export-all','-Wl,--export=__stack_pointer','-Wl,--initial-memory=1891631104',str(ROOT/'port/web/memory_fixture.c'),'-o',str(out/'memory-fixture-raw.wasm')],check=True,env=dict(os.environ,TEMP=str(out),TMP=str(out)))
# Construct the valid unpublished-frame leaf case explicitly; this compiler's
# fixture happens to publish SP even though the original leaf needs no calls.
m=Module((out/'memory-fixture-raw.wasm').read_bytes());bodies=list(m.bodies)
for k in range(len(bodies)):
    if m.name(m.imported_funcs+k)!='leaf':continue
    b,start=m.body_code(k);body=bytearray(b[:start])
    for at,op,sub,imm,end in instructions(b,start,len(b)):
        body+=b'\x1a' if op==0x24 else b[at:end]
    bodies[k]=bytes(body)
m.section(10)[1]=enc_u(len(bodies))+b''.join(enc_u(len(b))+b for b in bodies)
(out/'memory-fixture-raw.wasm').write_bytes(m.encode())
rewrite(out/'memory-fixture-raw.wasm',out/'memory-fixture.wasm',out/'memory.wasm')
# Omission control: make helpers reuse the C stack again. The leaf canary must fail.
m=Module((out/'memory-fixture.wasm').read_bytes())
count,_=uleb(m.section(6)[1],0);private=count-1;shared=exports(m)['__stack_pointer'][1]
bodies=[]
for k in range(len(m.bodies)):
    b,start=m.body_code(k);body=bytearray(b[:start])
    for at,op,sub,imm,end in instructions(b,start,len(b)):
        g,_=uleb(b,imm) if op in (0x23,0x24) else (None,None)
        body+=bytes([op])+enc_u(shared) if g==private else b[at:end]
    bodies.append(bytes(body))
m.section(10)[1]=enc_u(len(bodies))+b''.join(enc_u(len(b))+b for b in bodies)
(out/'memory-fixture-shared-stack.wasm').write_bytes(m.encode())
