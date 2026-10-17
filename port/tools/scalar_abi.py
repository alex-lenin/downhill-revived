"""Fail native calls that disagree with proven FP or wide scalar contracts."""
import json
from pathlib import Path
import re

FP={'float','double','long double'}
WIDE={'int64'}
QUAD={'int128'}
CATALOG=Path(__file__).resolve().parents[1]/'scalar_contracts.json'


def contracts():
    return json.loads(CATALOG.read_text())['assembly']


def records(nodes, decls, typedefs, signature, spelling, rel):
    out=[]; parents={}
    for n in nodes:
        for c in n.get('inner',[]):parents[id(c)]=n
    def unwrap(n):
        while n.get('kind') in ('ImplicitCastExpr','CStyleCastExpr','CXXStaticCastExpr','CXXReinterpretCastExpr','ParenExpr','UnaryOperator') and len(n.get('inner',[]))==1:n=n['inner'][0]
        return n
    def location(n):
        loc=n.get('range',{}).get('begin',{});loc=loc.get('expansionLoc',loc)
        return dict(line=loc.get('line'),offset=loc.get('offset'))
    for n in nodes:
        if n.get('kind') not in ('CallExpr','CXXMemberCallExpr') or not n.get('inner'):continue
        callee=n['inner'][0];ref=unwrap(callee)
        key=ref.get('referencedDecl',{}).get('id') or ref.get('referencedMemberDecl')
        d=decls.get(key)
        if not d:continue
        if n['kind']=='CXXMemberCallExpr':
            sig=d['signature'];actual=['void *']+[spelling(c.get('type',{})) for c in n['inner'][1:]]
        else:
            sig=signature(spelling(callee['type']),typedefs);actual=[spelling(c.get('type',{})) for c in n['inner'][1:]]
        # Unprototyped C calls use default-promoted expression types, not ().
        args=sig['args'] or actual
        if sig['variadic']:args=args[:len(sig['args'])]
        out.append(dict(file=rel,symbol=d['symbol'],ret=sig['ret'],args=args,
                        variadic=sig['variadic'],actual_args=actual,kind='call',**location(n)))
    for n in nodes:
        if n.get('kind') != 'CXXConstructExpr': continue
        match=re.search(r'\bfunc_[0-9A-F]{8}\b',spelling(n.get('type',{})))
        if not match or not n.get('ctorType'):continue
        sig=signature(spelling(n['ctorType']),typedefs)
        out.append(dict(file=rel,symbol=match[0],ret='void',args=['void *']+sig['args'],variadic=sig['variadic'],kind='constructor',**location(n)))
    # Exported callback arrays/pointers must have the same callable contract in
    # every unit, even when their initializer and call sites are in other files.
    for n in nodes:
        if n.get('kind') != 'VarDecl' or not n.get('name'):continue
        if parents.get(id(n),{}).get('kind') not in ('TranslationUnitDecl','LinkageSpecDecl'):continue
        t=spelling(n.get('type',{}))
        if not re.search(r'\(\s*\*[^)]*\)\s*\(',t):continue
        name=n['name']
        if n.get('storageClass')=='static':name=rel+':'+name
        sig=signature(re.sub(r'\(\s*\*[^)]*\)', '',t),typedefs)
        out.append(dict(file=rel,symbol='@table:'+name,ret=sig['ret'],args=sig['args'],variadic=sig['variadic'],kind='callback-table',**location(n)))
    # Check callback escapes and casts of indirect pointers as well as calls.
    for n in nodes:
        if n.get('kind') not in ('ImplicitCastExpr','CStyleCastExpr','CXXReinterpretCastExpr','CXXStaticCastExpr') or not n.get('inner'):continue
        dest=spelling(n.get('type',{}));src=spelling(n['inner'][0].get('type',{}))
        if not re.search(r'\(\s*\*[^)]*\)\s*\(',dest):continue
        target=unwrap(n);d=decls.get(target.get('referencedDecl',{}).get('id'))
        # Direct call casts are validated using their final callable type above.
        top=n
        while parents.get(id(top),{}).get('kind') in ('ImplicitCastExpr','CStyleCastExpr','CXXReinterpretCastExpr','CXXStaticCastExpr','ParenExpr','UnaryOperator'):top=parents[id(top)]
        p=parents.get(id(top),{})
        direct_call=p.get('kind') in ('CallExpr','CXXMemberCallExpr') and p.get('inner',[None])[0] is top
        if direct_call and d:continue
        sig=signature(re.sub(r'\(\s*\*[^)]*\)', '',dest),typedefs)
        record=dict(file=rel,symbol=d['symbol'] if d else '@indirect-cast',ret=sig['ret'],args=sig['args'],variadic=sig['variadic'],kind='callback/cast',**location(n))
        if not d:
            if not re.search(r'\(\s*\*[^)]*\)\s*\(',src):continue
            original=signature(re.sub(r'\(\s*\*[^)]*\)', '',src),typedefs)
            record['source_contract']=dict(ret=original['ret'],args=original['args'],variadic=original['variadic'])
        out.append(record)
    return out


def audit(units, definitions, abi):
    expected={name:{(abi(d['signature']['ret']),tuple(map(abi,d['signature']['args'])),d['signature']['variadic']) for d in ds} for name,ds in definitions.items()}
    for name,c in contracts().items():expected.setdefault(name,set()).add((c['ret'],tuple(c['args']),c.get('variadic',False)))
    for unit in units:
        for ref in unit[4] if len(unit)>4 else []:
            if ref['kind']=='callback-table':expected.setdefault(ref['symbol'],set()).add((abi(ref['ret']),tuple(map(abi,ref['args'])),ref['variadic']))
    fp=[];wide=[];quad=[]
    for unit in units:
        for ref in unit[4] if len(unit)>4 else []:
            choices=expected.get(ref['symbol'],set())
            if 'source_contract' in ref:
                c=ref['source_contract'];choices={(abi(c['ret']),tuple(map(abi,c['args'])),c['variadic'])}
            if not choices:continue
            if len(choices)>1 and not ref['symbol'].startswith('@table:'):continue
            old=tuple(map(abi,ref['args']));oldret=abi(ref['ret'])
            alternatives=choices-{(oldret,old,ref['variadic'])}
            ret,args,var=next(iter(alternatives or choices))
            pairs=list(zip(old,args));pairs += ([(x,'double' if x=='float' else x) for x in old[len(args):]] if var else [(x,'missing') for x in old[len(args):]])+ [('missing',x) for x in args[len(old):]]
            fpbad=any(a!=b and (a in FP or b in FP) for a,b in pairs)
            widebad=any(a!=b and (a in WIDE or b in WIDE) for a,b in pairs) or (oldret!=ret and (oldret in WIDE or ret in WIDE))
            quadbad=any(a!=b and (a in QUAD or b in QUAD) for a,b in pairs) or (oldret!=ret and (oldret in QUAD or ret in QUAD))
            record=dict(file=ref['file'],line=ref['line'],offset=ref['offset'],symbol=ref['symbol'],kind=ref['kind'],declared=list(old),defined=list(args),declared_return=oldret,defined_return=ret)
            if fpbad:fp.append(record)
            if widebad:wide.append(record)
            if quadbad:quad.append(record)
    key=lambda r:(r['file'],r['offset'] or 0,r['symbol'])
    return dict(float_argument_mismatches=sorted(fp,key=key),integer64_mismatches=sorted(wide,key=key),integer128_mismatches=sorted(quad,key=key))


def require_clean(report):
    errors=[(label,r) for key,label in [('float_argument_mismatches','FP arguments'),('integer64_mismatches','64-bit args/return'),('integer128_mismatches','128-bit args/return')] for r in report.get(key,[])]
    if errors:
        lines=['Scalar ABI guard: %d unsafe native calls/callbacks' % len(errors)]
        lines+=['%s:%s: %s: %s %s -> %s; return %s -> %s' % (r['file'],r['line'] or '?',r['symbol'],label,r['declared'],r['defined'],r['declared_return'],r['defined_return']) for label,r in errors]
        raise SystemExit('\n'.join(lines))


def verify_native_contracts(definitions, abi):
    """A reviewed bridge contract must track the native function definition."""
    native=json.loads((CATALOG.parent/'native_scalar_contracts.json').read_text())
    errors=[]
    for name,ds in definitions.items():
        if not name.startswith('func_') or ':' in name or name in native:continue
        for d in ds:
            sig=d['signature'];classes={abi(sig['ret'])}|set(map(abi,sig['args']))
            if classes & (FP|WIDE|QUAD):errors.append('%s: missing native scalar bridge contract' % name)
    for name,c in native.items():
        ds=definitions.get(name,[])
        actual={(abi(d['signature']['ret']),tuple(map(abi,d['signature']['args'])),d['signature']['variadic']) for d in ds}
        if not actual:continue  # small standalone guard fixtures omit game definitions
        expected=(c['ret'],tuple(c['args']),c['variadic'])
        if c['ret'] is None:actual={(None,a,v) for r,a,v in actual}
        if 'native_return' in c:
            spellings={re.sub(r'\b(const|volatile|signed)\b','',d['signature']['ret']).strip() for d in ds}
            if spellings!={c['native_return']}:errors.append('%s: stale small scalar return %s' % (name,spellings))
        if actual!={expected}:errors.append('%s: metadata %s != definition %s' % (name,expected,actual))
    if errors:raise SystemExit('Stale native scalar bridge contracts:\n'+'\n'.join(errors))


def verify_assembly_inputs(names):
    """Reject unreviewed incoming FPR/quad inputs at an asm/native boundary.
    Calls clobber caller-saved FPRs; varargs register-save areas are explicitly
    catalogued and are not treated as fixed floating arguments. This census is
    an additional guard, never the authority for inventing a C prototype.
    """
    root=CATALOG.parents[1]
    reviewed=contracts()
    errors=[]
    for name in sorted(names):
        path=root/'asm/nonmatchings/text'/(name+'.s')
        if not path.exists():continue
        dead=set();incoming=set();live={'a0','a1','a2','a3','t0','t1','t2','t3'};quads=set()
        for line in path.read_text().splitlines():
            match=re.search(r'/\* [0-9A-F]+ [0-9A-F]+ [0-9A-F]+ \*/\s+(\S+)\s*(.*)',line)
            if not match:continue
            op,args=match.groups()
            # Decode disassembler annotations on cvt.w.s words as well.
            if op=='.word':
                note=re.search(r'#\s+(\S+)\s+(.*?)(?:#|$)',args)
                if not note:continue
                op,args=note.groups()
            args=args.split('#',1)[0];parts=args.split(',')
            regs=[set(re.findall(r'\$f(\d+)\b',a)) for a in parts]
            written=set();read=set().union(*regs) if regs else set()
            if op=='mtc1':written=regs[1] if len(regs)>1 else set();read=set()
            elif op.startswith(('add.s','sub.s','mul.s','div.s','sqrt.s','rsqrt.s','max.s','min.s','mov.s','abs.s','neg.s','cvt.','lwc1','ldc1','madd.s','msub.s')):
                written=regs[0];read=set().union(*regs[1:]) if len(regs)>1 else set()
            incoming|={int(r) for r in read-dead if 12<=int(r)<=19}
            dead|=written
            gpr=re.search(r'\$(a[0-3]|t[0-3])\b',parts[0])
            if gpr and op in ('sq','qmtc2','qmtc2.ni') and gpr[1] in live:quads.add(gpr[1])
            stores=('sd','sw','sh','sb','sq','sqc2','swc1','sdc1','qmtc2','qmtc2.ni','mtc1','mult','multu','div','divu','jr','jalr')
            if gpr and op not in stores and not op.startswith(('b','c.')):live.discard(gpr[1])
            if op in ('jal','jalr'):dead|={str(i) for i in range(20)};live.clear()
        c=reviewed.get(name)
        if incoming and (not c or (not c.get('variadic') and c['args'].count('float')<max(incoming)-11)):
            errors.append('%s: unreviewed incoming FPRs %s' % (name,sorted(incoming)))
        if quads and (not c or 'int128' not in c['args']):errors.append('%s: unreviewed quadword argument registers %s' % (name,sorted(quads)))
    if errors:raise SystemExit('Assembly scalar input guard:\n'+'\n'.join(errors))


def verify_variadic_bridges(units, definitions, abi):
    """An asm vararg wrapper cannot guess a double/DI/TI value as uint32_t."""
    catalog=contracts();errors=[]
    for unit in units:
        for ref in unit[4] if len(unit)>4 else []:
            name=ref['symbol'];c=catalog.get(name)
            if name in definitions or not c or not c.get('variadic') or 'format_arg' in c:continue
            if any(abi(t) in (FP|WIDE|QUAD) for t in ref.get('actual_args',[])[len(c['args']):]):
                errors.append('%s:%s: %s needs a typed variadic bridge' % (ref['file'],ref['line'] or '?',name))
    if errors:raise SystemExit('Variadic scalar ABI guard:\n'+'\n'.join(errors))
