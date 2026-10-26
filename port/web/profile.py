"""Summarize Node --cpu-prof samples, resolving wasm indices without a browser.

Use the exact optimized module (built with -g) for each reactor. Categories
are exclusive self time, not inclusive attribution or subtractable speedups.
"""
import argparse,collections,json,re,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from wasm_postlink import Module

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('profile',type=Path)
    p.add_argument('--core',type=Path,required=True)
    p.add_argument('--graphics',type=Path,required=True)
    p.add_argument('--name',action='append',default=[],help='index=name for older unnamed helpers')
    a=p.parse_args();data=json.loads(a.profile.read_text())
    core=Module(a.core.read_bytes());graphics=Module(a.graphics.read_bytes())
    overrides={int(s.split('=',1)[0]):s.split('=',1)[1] for s in a.name}
    nodes={n['id']:n for n in data['nodes']}
    # V8 still records the wasm index in the frame even with a name section.
    indices={}
    for i,n in nodes.items():
        f=n['callFrame'];m=re.search(r'(?:wasm-function\[|\$)(\d+)\]?',f['functionName'])
        if m and f['url'].startswith('wasm:'):indices[i]=int(m[1])
    game_urls=collections.Counter(n['callFrame']['url'] for n in nodes.values() if n['callFrame']['functionName'].startswith('func_'))
    if game_urls:core_url=game_urls.most_common(1)[0][0]
    elif indices:core_url=nodes[max(indices,key=indices.get)]['callFrame']['url']
    else:
        urls=collections.Counter(n['callFrame']['url'] for n in nodes.values() if n['callFrame']['url'].startswith('wasm:'))
        if not urls:raise ValueError('profile contains no wasm frames')
        core_url=urls.most_common(1)[0][0]
    names={};categories={}
    for i,n in nodes.items():
        f=n['callFrame'];name=f['functionName'];category='JS/runtime/startup'
        if f['url'].startswith('wasm:'):
            is_core=f['url']==core_url;module=core if is_core else graphics
            if i in indices:name=(overrides.get(indices[i]) if is_core else None) or module.name(indices[i])
            if 'web_access' in name or any(s in name for s in ('web_prepare','web_finish','web_assert_unfolded')):category='memory adaptation'
            elif any(s in name for s in ('web_copy','web_fill')):category='bulk adaptation'
            elif 'web_helper' in name:category='float layer/builtins'
            elif 'XXH' in name:category='trace hash'
            else:category='core (includes inlined helpers/trace)'
            if not is_core:category='GS '+(category if category!='core (includes inlined helpers/trace)' else 'reactor/interpreter')
        elif any(s in name.lower() for s in ('activate','resume','microtask','promise','save')):category='JSPI scheduler/runtime'
        elif name.startswith('host_') or name in ('imports.env.<computed>','wasm-to-js','setCsr') or name.startswith('js-to-wasm:'):
            category='JS imports/boundary'
        names[i]=name;categories[i]=category
    totals=collections.Counter();top=collections.Counter()
    for i,delta in zip(data['samples'],data['timeDeltas']):
        totals[categories[i]]+=delta;top[names[i]]+=delta
    total=sum(totals.values())
    print(json.dumps({'sample_seconds':total/1e6,'core_url':core_url,
        'self_percent':{k:round(v*100/total,3) for k,v in totals.most_common()},
        'top_self_seconds':{k:round(v/1e6,6) for k,v in top.most_common(30)}},indent=2))
if __name__=='__main__':main()
