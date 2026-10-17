"""Census of inline asm in src/: which instruction sequences appear, how often, in how many files.
usage: python port/tools/asm_census.py"""
import ast, collections, glob, os, re, sys
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import source_rules as r  # noqa: E402

blocks, ops, files_with = collections.Counter(), collections.Counter(), collections.defaultdict(set)
for p in glob.glob(os.path.join(ROOT, 'src', '**', '*.c*'), recursive=True):
    text = r._clean(open(p, encoding='utf-8', errors='replace').read())
    for b in r.ASM.finditer(text):
        head = re.match(r'\s*((?:"(?:\\.|[^"\\])*"\s*)+)', text[b.end():])
        if not head:
            continue
        try:
            code = ''.join(ast.literal_eval(s) for s in r.STRING.findall(head[1]))
        except (ValueError, SyntaxError):
            continue
        lines = [re.sub(r'\s+', ' ', l.strip()) for l in re.split(r'[\n;]', code) if l.strip()]
        if len(lines) == 1 and re.fullmatch(r'[A-Za-z_.$][\w.$]*', lines[0]) and lines[0] not in r.ALLOWED:
            continue  # asm("symbol") label on a declaration
        key = ' ; '.join(lines)
        blocks[key] += 1
        files_with[key].add(os.path.basename(p))
        for l in lines:
            ops[l.split()[0]] += 1
print('asm blocks: %d, distinct sequences: %d' % (sum(blocks.values()), len(blocks)))
print('\nopcodes:')
for o, n in ops.most_common():
    print('  %-12s %d' % (o, n))
print('\ntop sequences:')
for k, n in blocks.most_common(40):
    print('  %5d %4d files  %s' % (n, len(files_with[k]), k[:150]))
