"""Audit direct calls against definitions across all PC translation units.

The EE allocates integer and float arguments independently. Only permutations
which preserve the order within each bank are repaired. Other ABI differences
are reported, never guessed. clang resolves typedefs, asm aliases and casts.
Run with LLVM on PATH; --check verifies the committed manifest is current.
Every invocation fails on unsafe float/double returns, including callbacks.
"""
import argparse
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
import difflib
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

import float_abi
import scalar_abi
import pctarget
from recomp import split_type

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/abi'
MANIFEST = ROOT / 'port/abi_calls.json'


def walk(node):
    yield node
    for child in node.get('inner', []):
        yield from walk(child)


def spelling(node):
    return node.get('desugaredQualType', node.get('qualType', ''))


def span(node):
    r = node.get('range', {})
    a, b = r.get('begin', {}), r.get('end', {})
    a, b = a.get('expansionLoc', a), b.get('expansionLoc', b)
    if 'offset' not in a or 'offset' not in b or 'includedFrom' in a:
        return None
    return a['offset'], b['offset'] + b.get('tokLen', 0)


def signature(t, typedefs):
    # The call expression has a function POINTER type; definitions do not.
    t = re.sub(r'\(\s*\*\s*\)', '', t)
    ret, args, variadic = split_type(t)
    def resolve(t):
        for _ in range(12):
            new = re.sub(r'\b\w+\b', lambda m: typedefs.get(m[0], m[0]), t)
            if new == t: break
            t = new
        return t
    return dict(ret=resolve(ret), args=[resolve(a) for a in args], variadic=variadic,
                local_ret=ret, local_args=args)


def abi(t):
    if '*' in t or '&' in t or '[' in t: return 'int'
    if re.fullmatch(r'[^()]+\([^()]*\)', t): return 'int'  # adjusted function parameter
    t = re.sub(r'\b(const|volatile|signed)\b', '', t).strip()
    if t in ('float', 'double', 'long double', 'void'): return t
    if t in ('__int128', 'unsigned __int128'): return 'int128'
    if 'long long' in t: return 'int64'
    if re.fullmatch(r'(unsigned\s*)?(int|char|short|long|bool|_Bool)?( int)?', t) or t.startswith('enum '):
        return 'int'
    return 'aggregate:' + t


def permutation(old, new):
    if old == new or Counter(old) != Counter(new) or set(old) != {'int', 'float'}:
        return None
    remaining = list(enumerate(old))
    result = []
    for kind in new:
        i, _ = next(p for p in remaining if p[1] == kind)
        result.append(i)
        remaining.remove((i, kind))
    return result


def inspect(path):
    rel = path.relative_to(OUT / 'src').as_posix()
    text = path.read_bytes()  # clang offsets are bytes, not Unicode characters
    cpp = path.suffix != '.c'
    cmd = ['clang', '--target=' + pctarget.TARGET, '-w', '-fsyntax-only',
           '-std=gnu++98' if cpp else '-std=gnu89', '-Wno-error=int-conversion',
           '-Wno-error=incompatible-pointer-types', '-Wno-error=implicit-function-declaration',
           '-Wno-error=incompatible-function-pointer-types', '-Wno-return-mismatch',
           '-I', str(ROOT / 'port/include'), '-I', str(ROOT / 'include'),
           '-Xclang', '-ast-dump=json', str(path)]
    p = subprocess.run(cmd, capture_output=True)
    if p.returncode:
        raise RuntimeError(rel + '\n' + p.stderr.decode(errors='replace'))
    tree = json.loads(p.stdout)
    nodes = list(walk(tree))
    typedefs = {n['name']: spelling(n['type']) for n in nodes
                if n.get('kind') == 'TypedefDecl' and 'type' in n}
    decls, definitions, calls, indirect = {}, [], [], 0
    for n in nodes:
        kind = n.get('kind')
        if kind not in ('FunctionDecl', 'CXXMethodDecl', 'CXXConstructorDecl', 'CXXDestructorDecl') or not n.get('name'): continue
        name = n['name']
        label = n.get('mangledName', '')
        # Explicit asm labels are final linker names. Otherwise retain the
        # retail func_X identity, shared by C and C++ units after link renaming.
        has_label = any(c.get('kind') == 'AsmLabelAttr' for c in n.get('inner', []))
        symbol = label.removeprefix('_') if has_label else name
        if n.get('storageClass') == 'static': symbol = rel + ':' + (label or symbol)
        d = dict(symbol=symbol, signature=signature(spelling(n['type']), typedefs), file=rel)
        if kind != 'FunctionDecl' and n.get('storageClass') != 'static':
            # Native C calls to retail C++ methods carry an explicit this.
            d['signature']['args'].insert(0, 'void *')
        decls[n['id']] = d
        if any(c.get('kind') == 'CompoundStmt' for c in n.get('inner', [])):
            definitions.append(d)
    # clang may only expose an implicit declaration inside referencedDecl.
    # GNU89 permits these calls, but their assumed int return is unsafe for FP.
    for n in nodes:
        ref = n.get('referencedDecl', {})
        if ref.get('kind') == 'FunctionDecl' and ref.get('id') not in decls:
            decls[ref['id']] = dict(symbol=ref['name'],
                signature=signature(spelling(ref['type']), typedefs), file=rel)
    for n in nodes:
        if n.get('kind') != 'CallExpr' or not n.get('inner'): continue
        callee = n['inner'][0]
        # Only unwrap casts/parentheses. Searching arbitrary child expressions
        # would mistake a function returning a callback for the called target.
        ref = callee
        while ref.get('kind') in ('ImplicitCastExpr', 'CStyleCastExpr', 'ParenExpr', 'UnaryOperator') and len(ref.get('inner', [])) == 1:
            ref = ref['inner'][0]
        decl = decls.get(ref.get('referencedDecl', {}).get('id')) if ref.get('kind') == 'DeclRefExpr' else None
        if not decl:
            indirect += 1
            continue
        s = span(n)
        if not s: continue
        parts = [span(c) for c in n['inner']]
        calls.append(dict(file=rel, symbol=decl['symbol'], signature=signature(spelling(callee['type']), typedefs),
                          unprototyped=not cpp and spelling(callee['type']).rstrip().endswith('()'),
                          actual_args=[spelling(c['type']) for c in n['inner'][1:]],
                          argc=len(n['inner']) - 1, start=s[0], end=s[1], parts=parts))
    return definitions, calls, indirect, float_abi.records(nodes, decls, typedefs, rel, signature, spelling), scalar_abi.records(nodes, decls, typedefs, signature, spelling, rel)


def source_site(call, cache):
    rel = call['file']
    if rel not in cache:
        raw = (ROOT / 'src' / rel).read_bytes().replace(b'\r\n', b'\n')
        generated = (OUT / 'src' / rel).read_bytes()
        a, b = raw.splitlines(True), generated.splitlines(True)
        mapping = {}
        for i, j, count in difflib.SequenceMatcher(None, a, b, autojunk=False).get_matching_blocks():
            for k in range(count): mapping[j + k] = i + k
        cache[rel] = raw, generated, a, mapping
    raw, generated, lines, mapping = cache[rel]
    start, end = call['start'], call['end']
    row = generated.count(b'\n', 0, start)
    col = start - (generated.rfind(b'\n', 0, start) + 1)
    fragment = generated[start:end]
    line = mapping.get(row)
    offset = sum(map(len, lines[:line])) + col if line is not None else -1
    if offset < 0 or raw[offset:offset + len(fragment)] != fragment:
        # An unrelated asm/global rewrite may change the same source line.
        # An exact, unique whole-call match still gives an unambiguous guard.
        if raw.count(fragment) != 1: return None
        offset = raw.index(fragment)
        line = raw.count(b'\n', 0, offset)
        col = offset - (raw.rfind(b'\n', 0, offset) + 1)
    return dict(file=rel, line=line + 1, column=col + 1, length=len(fragment),
                sha256=hashlib.sha256(fragment).hexdigest())


_REVIEWED_COLOR_CACHE = {}

def reviewed_packed_color(candidates):
    """One reviewed physical ABI equivalence, revalidated against current C++."""
    if len(candidates)!=1:return False
    d=candidates[0]
    if d['symbol']!='func_00161868' or d['signature']['local_args']!=['Color','float *']:return False
    path=OUT/'src'/d['file'];source=path.read_bytes()
    key=(str(path),hashlib.sha256(source).hexdigest())
    if key not in _REVIEWED_COLOR_CACHE:
        probe=source+b'\ntypedef char ReviewedColorWord[(sizeof(Color)==4 && __alignof__(Color)==4 && __is_pod(Color))?1:-1];\n'
        cmd=['clang','--target='+pctarget.TARGET,'-w','-x','c++','-std=gnu++98',
             '-S','-emit-llvm','-O0','-I',str(ROOT/'port/include'),'-I',str(ROOT/'include'),'-o','-','-']
        result=subprocess.run(cmd,input=probe,capture_output=True)
        line=next((line for line in result.stdout.decode(errors='replace').splitlines()
                   if line.startswith('define ') and 'func_00161868' in line),'')
        # On native i386 this POD is a four-byte stack byval, matching an SI slot.
        _REVIEWED_COLOR_CACHE[key]=(result.returncode==0 and 'i32' in line.split('@')[0]
            and (('byval(' in line and 'align 4' in line) or line.split('(',1)[-1].startswith('i32 '))
            and line.count(',')==1 and 'x86_' not in line)
    return _REVIEWED_COLOR_CACHE[key]

def classified_counts(warnings, reviewed):
    """Count flagged call sites by callee address, with an exclusive upper bound."""
    gameplay = 0
    for warning in warnings:
        match = re.fullmatch(r'func_([0-9A-Fa-f]{8})', warning['symbol'])
        if match and 0x110000 <= int(match[1], 16) < 0x1C0000:
            gameplay += 1
    return dict(real_remaining=len(warnings), reviewed_false_positives=len(reviewed),
                gameplay_real_remaining=gameplay)


def audit(units):
    definitions, calls, indirect = defaultdict(list), [], 0
    units = list(units)
    for unit in units:
        defs, sites, count = unit[:3]
        for d in defs: definitions[d['symbol']].append(d)
        calls.extend(sites)
        indirect += count
    fixes, warnings, reviewed, cache = [], [], [], {}
    actual_calls={(r['file'],r['symbol'],r.get('offset')):r.get('actual_args',[])
                  for unit in units for r in (unit[4] if len(unit)>4 else []) if r.get('kind')=='call'}
    compared, unresolved = 0, Counter()
    for c in calls:
        candidates = definitions.get(c['symbol'], [])
        if not candidates:
            unresolved[c['symbol']] += 1
            continue
        signatures = {(abi(d['signature']['ret']), tuple(map(abi, d['signature']['args'])), d['signature']['variadic']) for d in candidates}
        old = c['signature']
        src = list(map(abi, old['args']))
        compared += 1
        reason = None
        if len(signatures) != 1:
            reason = 'conflicting definitions'
            dst = []
        else:
            ret, dst, variadic = next(iter(signatures))
            dst = list(dst)
            if old['variadic'] != variadic or (not variadic and c['argc'] != len(dst)) or (variadic and c['argc'] < len(dst)):
                reason = 'argument count / variadic mismatch'
            elif abi(old['ret']) != ret:
                reason = 'return ABI mismatch'
            elif src != dst:
                reason = 'parameter ABI mismatch'
        if not reason: continue
        site = source_site(c, cache)
        record = dict(file=c['file'], line=site['line'] if site else None, symbol=c['symbol'],
                      declared=src, defined=dst, declared_return=abi(old['ret']),
                      definitions=[d['file'] for d in candidates], reason=reason)
        if len(signatures) == 1: record['defined_return'] = ret
        actual=list(map(abi,c.get('actual_args',actual_calls.get((c['file'],c['symbol'],c['start']),[]))))
        # Reviewed rule: GNU89 () omits the formal list, but actual expressions
        # already include default promotions. Only exact, SI-only calls qualify.
        no_proto=c.get('unprototyped',c['file'].endswith('.c') and not old['args'])
        if (reason=='parameter ABI mismatch' and no_proto and not src and
                actual==dst and actual and set(actual)=={'int'}):
            record['actual_arguments']=actual
            record['reviewed_reason']='GNU89 no-prototype call: promoted SI arguments, order and return match the definition.'
            reviewed.append(record)
            continue
        if (reason=='parameter ABI mismatch' and src==['int','int'] and
                c['symbol']=='func_00161868' and reviewed_packed_color(candidates)):
            record['reviewed_reason']='Color is a verified four-byte POD passed in one i386 stack slot; the packed SI declaration has the same ABI.'
            reviewed.append(record)
            continue
        order = permutation(src, dst) if reason in ('parameter ABI mismatch', 'return ABI mismatch') else None
        if order is not None and site and all(c['parts']):
            site.update(symbol=c['symbol'], order=order, ret=old['local_ret'],
                        types=old['local_args'], parts=[[a-c['start'], b-c['start']] for a,b in c['parts']])
            fixes.append(site)
            if reason == 'return ABI mismatch':
                record['parameter_order_fixed'] = True
                warnings.append(record)
        else:
            if order is not None: record['reason'] += ' (source range unavailable)'
            warnings.append(record)
    fixes.sort(key=lambda s: (s['file'], s['line'], s['column']))
    warnings.sort(key=lambda s: (s['file'], s['line'] or 0, s['symbol']))
    return dict(version=1, sites=fixes), dict(compared=compared, indirect_calls=indirect,
        unresolved_calls=dict(sorted(unresolved.items())), warnings=warnings,
        reviewed_false_positives=reviewed, raw_warning_count=len(warnings)+len(reviewed),
        classification=classified_counts(warnings, reviewed),
        float_return_mismatches=float_abi.audit(units, definitions, abi))


def main():
    ap = argparse.ArgumentParser(__doc__)
    ap.add_argument('--check', action='store_true')
    ap.add_argument('-j', type=int, default=8)
    args = ap.parse_args()
    subprocess.run([sys.executable, str(ROOT / 'port/tools/build.py'), '--abi-audit'], check=True, cwd=ROOT)
    paths = [OUT / 'src' / p.relative_to(ROOT / 'src') for p in sorted((ROOT / 'src').rglob('*'))
             if p.suffix in ('.c', '.cpp')]
    with ThreadPoolExecutor(args.j) as pool:
        manifest, report = audit(pool.map(inspect, paths))
    # Audit actual native calls after the candidate safe EE-bank permutations.
    # Do not publish a manifest which has passed only the unfixed-source audit.
    candidate=OUT/'candidate.json'
    candidate.write_text(json.dumps(manifest,indent=2)+'\n')
    subprocess.run([sys.executable,str(ROOT/'port/tools/build.py'),'--scalar-audit',
                    '--abi-manifest',str(candidate),'-j',str(args.j)],check=True,cwd=ROOT)
    native_out=ROOT/'build/port/abi-native'
    native_paths=[native_out/'src'/p.relative_to(OUT/'src') for p in paths]
    original_out=OUT
    globals()['OUT']=native_out
    try:
        with ThreadPoolExecutor(args.j) as pool:native_units=list(pool.map(inspect,native_paths))
    finally:globals()['OUT']=original_out
    definitions=defaultdict(list)
    for unit in native_units:
        for definition in unit[0]:definitions[definition['symbol']].append(definition)
    report.update(scalar_abi.audit(native_units,definitions,abi))
    scalar_abi.verify_native_contracts(definitions,abi)
    scalar_abi.verify_variadic_bridges(native_units,definitions,abi)
    assembly_names={ref['symbol'] for unit in native_units for ref in unit[4]}-set(definitions)
    scalar_abi.verify_assembly_inputs(assembly_names)
    (OUT / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    float_abi.require_clean(report)
    scalar_abi.require_clean(report)
    data = json.dumps(manifest, indent=2) + '\n'
    if args.check:
        if MANIFEST.read_text() != data: raise SystemExit('ABI manifest is stale; run abi_check.py')
    else:
        MANIFEST.write_text(data)
    print('%d units; %d calls compared; %d reorder sites; %d other mismatches' %
          (len(paths), report['compared'], len(manifest['sites']), len(report['warnings'])))
    print('%d reviewed false positives (one-line reasons in report.json)' % len(report['reviewed_false_positives']))
    print('%d real flags in gameplay callee range [0x110000, 0x1C0000)' % report['classification']['gameplay_real_remaining'])
    print('Other mismatches and unresolved targets: build/port/abi/report.json')


if __name__ == '__main__':
    main()
