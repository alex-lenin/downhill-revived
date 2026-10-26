"""Canonicalize wasm C++ object symbols: C++ functions and globals get their retail names back.

A C++ function is emitted under a mangled name (_Z13func_00123456P3Obj) that depends on the
declaration a unit happened to see, while the data, the C units and the PS2 itself know it as
func_00123456. Every symbol whose demangled identifier is a retail name (func_XXXXXXXX,
D_XXXXXXXX) is renamed to that name. A class named after its address (class func_00166F28) gets
two constructors, complete (C1) and base (C2), and up to three destructors; gcc 2.95 had one of
each, so the retail name goes to the complete one. The other variants keep their mangled names
where a unit defines them; a variant that is only called gets the retail name.

LLVM objcopy cannot redefine wasm symbols. Rewrite only names in imports,
linking symbol records and the debug name map. Function indices, code, data
and COMDAT membership remain unchanged. Compatible aliases merge symbol
records and retarget relocation symbol indices, preserving offsets/addends.
"""
import subprocess
import re
from pathlib import Path

from wasm_postlink import Module, enc_u, name_bytes, uleb, sleb

RETAIL = re.compile(r'^(?:func|D)_[0-9A-F]{8}$')
VARIANT = re.compile(r'\d(?:func|D)_[0-9A-F]{8}(?:C2|D2|D0)E')  # base ctor/dtor, deleting dtor


def retail_name(demangled):
    """func_00123456 for 'ns::Obj::func_00123456(Obj*, int)', or None."""
    base = demangled.split('(')[0].split('::')[-1].strip()
    return base if RETAIL.match(base) else None


def string(data, at):
    size, start = uleb(data, at)
    return data[start:start + size].decode('utf-8'), start + size


def symbol_records(payload):
    """Yield (kind, flags, prefix bytes, optional name, suffix bytes)."""
    count, at = uleb(payload, 0)
    records = []
    for _ in range(count):
        start = at
        kind = payload[at]; flags, at = uleb(payload, at + 1)
        name = None
        if kind in (0, 2, 4, 5):  # function, global, tag, table
            _, at = uleb(payload, at)
            prefix = payload[start:at]
            if not flags & 0x10 or flags & 0x40:
                name, at = string(payload, at)
            suffix = b''
        elif kind == 1:  # data: name, then segment/offset/size when defined
            prefix = payload[start:at]
            name, at = string(payload, at)
            tail = at
            if not flags & 0x10:
                for _ in range(3): _, at = uleb(payload, at)
            suffix = payload[tail:at]
        elif kind == 3:  # section
            _, at = uleb(payload, at)
            prefix, suffix = payload[start:at], b''
        else:
            raise ValueError('unsupported wasm linking symbol kind %d' % kind)
        records.append((kind, flags, prefix, name, suffix))
    if at != len(payload):
        raise ValueError('trailing bytes in wasm symbol table')
    return records


def rename(path, cxxfilt='llvm-cxxfilt', thunks=None):
    """Rename in place; optionally collect required (base, caller)->callee ABIs.

    An incompatible local prototype stays a distinct import of base__as_SIG.
    The caller must emit those adapters using its ordinary ABI thunk stage.
    Without a collector this case is rejected before writing the object.
    """
    path = Path(path)
    module = Module(path.read_bytes())
    symbols, imports = [], []
    for sid, data, cname in module.sections:
        if cname == 'linking':
            _, at = string(data, 0)
            version, at = uleb(data, at)
            if version != 2: raise ValueError('unsupported wasm linking version')
            while at < len(data):
                sub = data[at]; size, start = uleb(data, at + 1); at = start + size
                if sub == 8: symbols = symbol_records(data[start:at])
        elif sid == 2:
            count, at = uleb(data, 0)
            for _ in range(count):
                namespace, at = string(data, at); name, at = string(data, at)
                start = at; kind = data[at]; at += 1
                if kind in (0, 4):
                    if kind == 4: at += 1
                    _, at = uleb(data, at)
                elif kind in (1, 2):
                    if kind == 1: at += 1
                    flags, at = uleb(data, at); _, at = uleb(data, at)
                    if flags & 1: _, at = uleb(data, at)
                elif kind == 3: at += 2
                else: raise ValueError('unsupported wasm object import kind')
                imports.append((namespace, name, data[start:at]))
            if at != len(data): raise ValueError('trailing import bytes')
    defined = {name for _, flags, _, name, _ in symbols if name and not flags & 0x10}
    candidates = sorted({name for _, _, _, name, _ in symbols if name and name.startswith('_Z')} |
                        {name for _, name, _ in imports if name.startswith('_Z')})
    candidates = [name for name in candidates if not (VARIANT.search(name) and name in defined)]
    mapping = {}
    if candidates:
        result = subprocess.run([str(cxxfilt)], input='\n'.join(candidates)+'\n',
                                capture_output=True, text=True, check=True)
        demangled = result.stdout.splitlines()
        if len(demangled) != len(candidates): raise ValueError('cxxfilt output count')
        def canonical(dem):
            base = dem.split('(')[0].split('::')[-1].strip()
            return retail_name(dem) or (base if re.fullmatch(r'func_[0-9A-F]{8}__as_[ilfd]*_[ilfdv]',base) else None)
        mapping = {name: canonical(dem) for name, dem in zip(candidates, demangled) if canonical(dem)}
    # A merged C++ TU can import a retail prototype and define its mangled
    # overload with the SAME wasm signature. Canonicalization unifies them.
    # Symbol records cannot share names, so merge the symbol references too.
    import_names = {0: [], 2: [], 4: [], 5: []}
    for _, name, tail in imports:
        kind = {0: 0, 3: 2, 4: 4, 1: 5}.get(tail[0])
        if kind is not None: import_names[kind].append(name)
    groups = {}
    def index(record):
        _, pos = uleb(record[2], 1)
        return uleb(record[2], pos)[0]
    for i, record in enumerate(symbols):
        kind, flags, _, name, _ = record
        if name is None and flags & 0x10 and kind in import_names:
            name = import_names[kind][index(record)]
        if name is not None: groups.setdefault(mapping.get(name,name), []).append(i)
    representatives = list(range(len(symbols)))
    import_overrides = {}
    symbol_overrides = {}
    aliases = 0
    for name, ids in groups.items():
        if len(ids) == 1: continue
        definitions = [i for i in ids if not symbols[i][1] & 0x10]
        keep = definitions[0] if definitions else ids[0]
        if any(symbols[i][0] != 0 for i in ids):
            raise ValueError('duplicate non-function canonical symbol '+name)
        if any(index(symbols[i]) != index(symbols[keep]) for i in definitions):
            raise ValueError('multiple definitions of canonical symbol '+name)
        target_type = module.ftype(index(symbols[keep]))
        for i in ids:
            call_type = module.ftype(index(symbols[i]))
            if call_type == target_type:
                representatives[i] = keep
                continue
            if thunks is None or not definitions or not symbols[i][1] & 0x10:
                raise ValueError('incompatible signatures inside canonical object: '+name)
            from wasm_postlink import VALTYPE, TAG
            def signature(t):
                return (tuple(VALTYPE[x] for x in t[0]), VALTYPE[t[1][0]] if t[1] else None)
            csig, dsig = signature(call_type), signature(target_type)
            alias = name+'__as_'+''.join(TAG[x] for x in call_type[0])+'_'+(TAG[call_type[1][0]] if call_type[1] else 'v')
            key = (name, csig)
            if key in thunks and thunks[key] != dsig:
                raise ValueError('inconsistent target signature for '+alias)
            thunks[key] = dsig
            import_overrides[index(symbols[i])] = alias
            symbol_overrides[i] = alias
            aliases += 1
    kept = [i for i, target in enumerate(representatives) if i == target]
    positions = {old: new for new, old in enumerate(kept)}
    remap = {old: positions[target] for old, target in enumerate(representatives)}
    merges = len(symbols)-len(kept)
    if not mapping and not merges and not aliases: return 0
    symbols = [(kind, flags, prefix, symbol_overrides.get(i,name) if name is not None else None, suffix)
               for i in kept for kind,flags,prefix,name,suffix in [symbols[i]]]
    def mapped_name(name): return name_bytes(mapping.get(name, name))
    output = bytearray(b'\0asm\x01\0\0\0')
    for sid, data, cname in module.sections:
        if sid == 2:
            entries = []; function = 0
            for ns,n,tail in imports:
                name = name_bytes(import_overrides[function]) if tail[0] == 0 and function in import_overrides else mapped_name(n)
                entries.append(name_bytes(ns)+name+tail)
                if tail[0] == 0: function += 1
            data = enc_u(len(imports))+b''.join(entries)
        elif cname == 'linking':
            _, at = string(data, 0); _, at = uleb(data, at)
            out = bytearray(data[:at])
            while at < len(data):
                sub = data[at]; size, start = uleb(data, at + 1); at = start + size
                body = data[start:at]
                if sub == 8:
                    body = enc_u(len(symbols)) + b''.join(prefix+(mapped_name(name) if name is not None else b'')+suffix
                                                        for _,_,prefix,name,suffix in symbols)
                elif sub == 6 and merges:  # init functions: priority, symbol
                    count, pos = uleb(body, 0); entries = []
                    for _ in range(count):
                        priority, pos = uleb(body, pos); symbol, pos = uleb(body, pos)
                        entries.append(enc_u(priority)+enc_u(remap[symbol]))
                    if pos != len(body): raise ValueError('trailing init-function bytes')
                    body = enc_u(count)+b''.join(entries)
                out += bytes([sub])+enc_u(len(body))+body
            data = bytes(out)
        elif cname and cname.startswith('reloc.') and merges:
            _, at = string(data, 0)
            _, at = uleb(data, at)  # target section index
            count, at = uleb(data, at)
            out = bytearray(data[:at])
            with_addend = {3,4,5,8,9,11,14,15,16,17,21,22,23,25}
            for _ in range(count):
                start = at; kind = data[at]; at += 1
                if kind > 26: raise ValueError('unsupported wasm relocation kind')
                _, at = uleb(data, at)  # offset stays unchanged
                out += data[start:at]
                symbol, at = uleb(data, at)
                out += enc_u(symbol if kind == 6 else remap[symbol])
                start = at
                if kind in with_addend: _, at = sleb(data, at)
                out += data[start:at]
            if at != len(data): raise ValueError('trailing relocation bytes')
            data = bytes(out)
        elif cname == 'name':
            _, at = string(data, 0); out = bytearray(data[:at])
            while at < len(data):
                sub = data[at]; size, start = uleb(data, at + 1); at = start + size
                body = data[start:at]
                if sub in (1, 7):
                    count, pos = uleb(body, 0); entries = []
                    for _ in range(count):
                        index, pos = uleb(body, pos); name, pos = string(body, pos)
                        entries.append(enc_u(index)+mapped_name(name))
                    if pos != len(body): raise ValueError('trailing name map bytes')
                    body = enc_u(count)+b''.join(entries)
                out += bytes([sub])+enc_u(len(body))+body
            data = bytes(out)
        output += bytes([sid])+enc_u(len(data))+data
    path.write_bytes(output)
    return len(mapping)+merges+aliases
