"""Strict native floating-return guard, including address-taken callbacks.

C definitions establish their own return contracts. Assembly-only contracts are
reviewed against retail code in port/float_returns.json; never infer a return
merely because a routine happens to use f0 as a scratch register.
"""
import json
from pathlib import Path
import re

FP = {'float', 'double', 'long double'}
CATALOG = Path(__file__).resolve().parents[1] / 'float_returns.json'


def assembly_returns():
    return {n: d['return'] for n, d in json.loads(CATALOG.read_text())['assembly'].items()}


def records(nodes, decls, typedefs, rel, signature, spelling):
    out = []
    parents = {}
    for n in nodes:
        for child in n.get('inner', []): parents[id(child)] = n
    def add(n, d, t, kind):
        loc = n.get('loc', n.get('range', {}).get('begin', {}))
        loc = loc.get('expansionLoc', loc)
        out.append(dict(file=rel, symbol=d['symbol'], signature=signature(t, typedefs),
                        kind=kind, line=loc.get('line'), offset=loc.get('offset'),
                        included=bool(loc.get('includedFrom'))))
    for n in nodes:
        d = decls.get(n.get('id'))
        if d:
            add(n, d, spelling(n['type']), 'declaration')
        if n.get('kind') != 'DeclRefExpr': continue
        d = decls.get(n.get('referencedDecl', {}).get('id'))
        if not d: continue
        top = n
        while parents.get(id(top), {}).get('kind') in (
                'ImplicitCastExpr', 'CStyleCastExpr', 'CXXReinterpretCastExpr',
                'CXXStaticCastExpr', 'ParenExpr', 'UnaryOperator'):
            top = parents[id(top)]
            t = spelling(top.get('type', {}))
            if re.search(r'\(\s*\*\s*\)', t):
                add(top, d, t, 'function-pointer conversion')
    for n in nodes:
        if n.get('kind') not in ('ImplicitCastExpr', 'CStyleCastExpr',
                'CXXReinterpretCastExpr', 'CXXStaticCastExpr') or not n.get('inner'): continue
        dest = spelling(n.get('type', {}))
        src = spelling(n['inner'][0].get('type', {}))
        if not all(re.search(r'\(\s*\*\s*\)', x) for x in (src, dest)): continue
        original = signature(src, typedefs)['ret']
        changed = signature(dest, typedefs)['ret']
        if original in FP and changed != original:
            add(n, {'symbol': '@pointercast'}, dest, 'indirect function-pointer cast')
            out[-1]['return_contract'] = original
    for n in nodes:
        if n.get('kind') != 'VarDecl' or not n.get('name'): continue
        parent = parents.get(id(n), {})
        if parent.get('kind') not in ('TranslationUnitDecl', 'LinkageSpecDecl'): continue
        t = spelling(n.get('type', {}))
        if not re.search(r'\(\s*\*[^)]*\)\s*\(', t): continue
        label = n.get('mangledName', n['name'])
        symbol = label.removeprefix('_') if any(c.get('kind') == 'AsmLabelAttr' for c in n.get('inner', [])) else n['name']
        if n.get('storageClass') == 'static': symbol = rel + ':' + symbol
        normalized = re.sub(r'\(\s*\*[^)]*\)', '', t)
        add(n, {'symbol': '@table:' + symbol}, normalized, 'callback-table declaration')
    # Repeated declarations/references need only one diagnostic per source location/type.
    unique = {}
    for d in out:
        key = (d['symbol'], d['signature']['ret'], d['kind'], d['offset'], d['included'], d.get('return_contract'))
        unique[key] = d
    return list(unique.values())


def audit(units, definitions, abi):
    expected = {n: {abi(d['signature']['ret']) for d in ds} for n, ds in definitions.items()}
    for n, ret in assembly_returns().items():
        expected.setdefault(n, set()).add(ret)
    for unit in units:
        for ref in unit[3] if len(unit) > 3 else []:
            if ref['symbol'].startswith('@table:'):
                expected.setdefault(ref['symbol'], set()).add(abi(ref['signature']['ret']))
    errors = []
    for unit in units:
        refs = unit[3] if len(unit) > 3 else []
        for ref in refs:
            returns = {ref['return_contract']} if 'return_contract' in ref else expected.get(ref['symbol'], set())
            declared = abi(ref['signature']['ret'])
            # Detect both missing x87 pops and reading an x87 result from a void/int callee.
            if returns & FP and len(returns) > 1:
                errors.append(dict(file=ref['file'], line=ref['line'], offset=ref['offset'],
                                   symbol=ref['symbol'], declared_return=declared,
                                   defined_return='/'.join(sorted(returns)), kind=ref['kind']))
            elif len(returns) == 1 and (returns & FP or declared in FP) and declared not in returns:
                errors.append(dict(file=ref['file'], line=ref['line'], offset=ref['offset'],
                                   symbol=ref['symbol'], declared_return=declared,
                                   defined_return=next(iter(returns)), kind=ref['kind']))
    return sorted(errors, key=lambda d: (d['file'], d['offset'] or 0, d['symbol'], d['kind']))


def require_clean(report):
    errors = report.get('float_return_mismatches', [])
    if errors:
        lines = ['Floating-return ABI guard: %d unsafe declarations/conversions' % len(errors)]
        lines += ['%s:%s: %s: %s -> %s (%s)' %
                  (d['file'], d['line'] or '?', d['symbol'], d['declared_return'],
                   d['defined_return'], d['kind']) for d in errors]
        raise SystemExit('\n'.join(lines))
