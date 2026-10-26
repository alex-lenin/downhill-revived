"""Generate and validate JS import metadata against host_api.h (no guessed ABI)."""
import json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'port/tools'))
from wasm_postlink import Module,uleb
from gen_host64_imports import api_declarations,wasm_type,ctype,NATIVE_ONLY
def generate(module,out):
    m=Module(Path(module).read_bytes());p=m.section(2)[1];n,i=uleb(p,0);rows={};f=0
    api=api_declarations(ROOT/'port/host/host_api.h')
    tag={0x7f:'u32',0x7e:'u64',0x7d:'f32',0x7c:'f64'}
    for _ in range(n):
        ln,i=uleb(p,i);ns=p[i:i+ln].decode();i+=ln
        ln,i=uleb(p,i);name=p[i:i+ln].decode();i+=ln
        kind=p[i];t,i=uleb(p,i+1)
        if kind!=0 or ns!='env' or name not in api or name in NATIVE_ONLY:
            raise ValueError(f'unsupported import {ns}.{name}')
        params,results=m.types[t];ret,args=api[name]
        actual=[tag[results[0]] if results else 'void',*[tag[t] for t in params]]
        expected=[wasm_type(ret),*[wasm_type(ctype(a)) for a in args]]
        if expected!=actual:raise ValueError(f'{name}: {actual} != {expected}')
        rows[name]={'result':actual[0],'params':actual[1:]};f+=1
    Path(out).write_text(json.dumps(rows,indent=2)+'\n');print(f'{f} imports match host_api.h')
if __name__=='__main__':generate(*sys.argv[1:])
