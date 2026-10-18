"""Translate straight-line VU1 blocks from the user's local microcode into C.

The generated include is game-derived data and belongs only under build/.
This first tier removes instruction decoding; pipeline state remains in the
reference runtime. Unknown or modified blocks use the interpreter unchanged.
No upstream implementation is copied.

A block runs only where micro memory holds the words it was translated from. dhd.exe doesn't
keep a copy of them: the include lists where each block's words are in .vutext, and
ps2_vu_aot_load (port/runtime/vu0_interp.c) copies them from the data image read from the
player's disc. The test and bench programs, which have no disc image, define
PS2_VU_AOT_EMBED and get the words as constants.
"""
from pathlib import Path
import os
import struct

from vudis import BASE, walk

ROOT = Path(__file__).resolve().parents[2]


def ends_block(lo, hi):
    if hi & (1 << 30):
        return True
    if hi >> 31:
        return False
    # XGKICK is a boundary: its callback may inspect/change micro memory.
    # Simple branch/E delay slots can be fused by blocks().
    return lo >> 25 in (0x20, 0x21, 0x24, 0x25, 0x28, 0x29, 0x2c, 0x2d, 0x2e, 0x2f) or (
        lo >> 25 == 0x40 and (lo & 63) >= 60 and ((lo >> 6 & 31) * 4 + (lo & 3)) == 0x6c)


def blocks(pairs):
    starts = set()
    for pc, (lo, hi) in pairs.items():
        if hi & (1 << 30):
            starts.add(pc + 2)  # next program after the E delay slot
        if not hi >> 31 and lo >> 25 in (0x20, 0x21, 0x24, 0x25, 0x28, 0x29, 0x2c, 0x2d, 0x2e, 0x2f):
            starts.add(pc + 2)  # conditional fallthrough / return address
            if lo >> 25 not in (0x24, 0x25):
                immediate = lo & 0x7ff
                if immediate & 0x400:
                    immediate -= 0x800
                starts.add((pc + 1 + immediate) & 0x7ff)
    result = {}
    at = min(pairs)
    while at <= max(pairs):
        if at not in pairs:
            at += 1
            continue
        start, block = at, []
        while at in pairs and len(block) < 32:
            pair = pairs[at]
            block.append(pair)
            at += 1
            if ends_block(*pair) or at in starts:
                break
        # Fuse a control transfer with its simple delay slot. The run loop
        # completes that transition immediately after this block returns.
        if ends_block(*block[-1]) and at in pairs:
            lo, hi = block[-1]
            branch = not hi >> 31 and lo >> 25 in (0x20, 0x21, 0x24, 0x25, 0x28, 0x29, 0x2c, 0x2d, 0x2e, 0x2f)
            if (branch or hi & (1 << 30)) and not ends_block(*pairs[at]):
                block.append(pairs[at])
                # Still create an independent block for the slot: an entry or
                # another branch may target it without the preceding branch.
        result[start] = tuple(block)
    return result


def delay_slots(pairs):
    """Single pairs for the slots after a branch or an E bit, where the interpreter does not
    enter straight-line blocks: the run loop keeps the branch and end transitions."""
    result = {}
    for pc, (lo, hi) in pairs.items():
        branch = not hi >> 31 and lo >> 25 in (0x20, 0x21, 0x24, 0x25, 0x28, 0x29, 0x2c, 0x2d, 0x2e, 0x2f)
        if (branch or hi & (1 << 30)) and pc + 1 in pairs:
            result[pc + 1] = (pairs[pc + 1],)
    return result


def program(data, package):
    """{pc: (lo, hi)} of one microcode package, and {pc: PS2 address of that pair in .vutext}."""
    pairs, where = {}, {}
    for address, chunk, source in walk(data, package, sources=True)[1]:
        for offset, pair in enumerate(struct.iter_unpack('<II', chunk)):
            pairs[address // 8 + offset] = pair
            where[address // 8 + offset] = source + 8 * offset
    return pairs, where


def fills(name, data, sources, words):
    """Copy entries (name, word offset, PS2 address, word count) for an array of the given
    words, whose pairs are at sources in .vutext; checked against data."""
    out = []
    for k, source in enumerate(sources):
        if out and out[-1][2] + 4 * out[-1][3] == source:
            out[-1] = out[-1][:3] + (out[-1][3] + 2,)
        else:
            out.append((name, 2 * k, source, 2))
    for _, at, source, count in out:
        got = struct.unpack_from('<%dI' % count, data, source - BASE)
        if list(got) != list(words[at:at + count]):
            raise ValueError('%s: the words at 0x%08X are not the translated ones' % (name, source))
    return out


WORDS_MACRO = ['#ifndef AOT_WORDS',
               '#ifdef PS2_VU_AOT_EMBED',
               '#define AOT_WORDS(name, count, ...) static const uint32_t name[count] = {__VA_ARGS__};',
               "#else  /* filled by ps2_vu_aot_load from the game's .vutext */",
               '#define AOT_WORDS(name, count, ...) static uint32_t name[count];',
               '#endif',
               '#endif']


def fill_function(name, entries):
    """C of the function that copies the words in (entries of fills), without PS2_VU_AOT_EMBED."""
    lines = ['#ifndef PS2_VU_AOT_EMBED', 'static const struct { uint32_t *words; unsigned at, source, count; } '
             '%s_fill[] = {' % name]
    lines += ['    {%s, %d, 0x%08X, %d},' % e for e in entries]
    lines += ['};', 'static void %s_load(const uint8_t *vutext) {' % name,
              '    for (unsigned i = 0; i < sizeof %s_fill / sizeof *%s_fill; i++)' % (name, name),
              '        memcpy(%s_fill[i].words + %s_fill[i].at, vutext + (%s_fill[i].source - 0x%X), %s_fill[i].count * 4);'
              % (name, name, name, BASE, name), '}', '#endif']
    return lines


def emit_block(prefix, i, block, unit):
    """C for one straight-line block: the interpreter's steps for each pair with the words as
    constants (pair_stalls only on VU1, as in the interpreter)."""
    hint = 'VU1(u); ' if unit else 'VU0(u); '
    fused = len(block) > 1 and ends_block(*block[-2])
    fused_end = fused and bool(block[-2][1] & (1 << 30))
    lines = [f'static int {prefix}_block_{i}(Vu *u, unsigned pc, int *bad) {{',
             '    Upper up; int next = -1, transfer = -1;', f'    u->unit = {unit};']
    for offset, (lo, hi) in enumerate(block):
        lines += ['    {']
        if unit:
            lines += [f'    VU1(u); pair_stalls(u, 0x{hi:08X}u, 0x{lo:08X}u);']
        lines += [f'    {hint}apply_flags(u); update_q(u);',
                  f'    {hint}if (upper(u, 0x{hi:08X}u, &up) < 0) {{ *bad = 1; return -1; }}']
        if hi >> 31:
            lines += [f'    const uint32_t immediate = 0x{lo:08X}u;',
                      '    memcpy(&u->c->i, &immediate, 4);']
        else:
            lines += [f'    if (!(up.flags == 2 && {lo >> 25} == 0x11)) {{',
                      f'        {hint}next = lower(u, 0x{lo:08X}u, pc + {offset}, bad);', '    }']
        lines += ['    if (*bad) return -1;', f'    {hint}commit_upper(u, &up, {lo >> 16 & 31});']
        if fused and offset == len(block) - 2:
            lines += ['    transfer = next;']
        if offset != len(block) - 1:
            lines += [f'    trace_pair(u, pc + {offset});', '    u->cycle++;']
        lines += ['    }']
    lines += [f'    return {"transfer" if fused else "next"};', '}',
              f'AOT_WORDS({prefix}_words_{i}, {2 * len(block)}, ' + ','.join(f'0x{w:08X}u' for pair in block for w in pair) + ')',
              f'static const AotBlock {prefix}_info_{i} = {{{len(block)}, {2 if fused_end else 1 if fused else 0}, '
              f'{prefix}_words_{i}, {prefix}_block_{i}}};']
    return lines


def generate(destination):
    destination = Path(destination).resolve()
    if ROOT / 'build' not in destination.parents:
        raise ValueError('game-derived AOT output must stay under build/')
    data = Path(os.environ.get('DHD_VUTEXT', ROOT / 'assets/vutext.bin')).read_bytes()
    overlays, found = [], {}
    for package in (0x265C00, 0x269C50):
        pairs, where = program(data, package)
        overlays.append((blocks(pairs), delay_slots(pairs)))
        for table in overlays[-1]:
            for pc, block in table.items():
                found.setdefault(block, [where[pc + k] for k in range(len(block))])
    unique = sorted(set(block for overlay in overlays for table in overlay for block in table.values()))
    ids = {block: i for i, block in enumerate(unique)}
    lines = ['/* Generated from local microcode. Do not commit. */',
             '/* fused: 0 = ordinary block, 1 = branch + slot, 2 = E + slot. */',
             'typedef struct { unsigned count, fused; const uint32_t *words; int (*run)(Vu *, unsigned, int *); } AotBlock;']
    lines += WORDS_MACRO
    entries = []
    for i, block in enumerate(unique):
        lines += emit_block('aot', i, block, 1)
        entries += fills('aot_words_%d' % i, data, found[block], [w for pair in block for w in pair])
    lines += ['static const AotBlock *const aot_blocks[2][2][2048] = {  /* [overlay][delay slot][pc] */']
    for overlay in overlays:
        lines.append(' {')
        for table in overlay:
            lines.append('  {')
            lines += [f'   &aot_info_{ids[table[pc]]},' if pc in table else '   NULL,' for pc in range(2048)]
            lines.append('  },')
        lines.append(' },')
    # A block runs only where micro memory holds its exact words. In the game's VU1 micro memory
    # (0x11008000, changed only by VIF1 MPG, which bumps ps2_vu1_code_version) the block found
    # (or none) is remembered until the next change; any other code buffer (the oracle tests)
    # compares each time.
    lines += ['};', 'static struct { uint64_t version; const AotBlock *block; } aot_resolved[2][2048];  /* version + 1 */',
              'static const AotBlock *aot_find(unsigned pc, const uint32_t *words, unsigned remaining, int slot) {',
              '    const int game = (uintptr_t)(words - pc * 2) == 0x11008000;',
              '    const AotBlock *found = NULL;',
              '    if (game && aot_resolved[slot][pc].version == ps2_vu1_code_version + 1)',
              '        found = aot_resolved[slot][pc].block;',
              '    else {',
              '        for (unsigned overlay = 0; overlay < 2 && !found; overlay++) {',
              '            const AotBlock *block = aot_blocks[overlay][slot][pc];',
              '            if (block && !memcmp(words, block->words, block->count * 8)) found = block;',
              '        }',
              '        if (game) aot_resolved[slot][pc].version = ps2_vu1_code_version + 1, aot_resolved[slot][pc].block = found;',
              '    }',
              '    return found && found->count <= remaining ? found : NULL;', '}']
    # VU0 micro mode: the game's one VU0 package (finding 019: 426 instructions at 0x26CF80), run by
    # vcallms from the EE. The interpreter has no stall model for VU0; the blocks skip it too.
    # VU0 micro memory can be written by any EE store as well as VIF0 MPG, so the words are
    # compared on every dispatch (blocks are short) instead of being remembered.
    pairs0, where0 = program(data, 0x26CF80)
    # The complete native VU0 culling entry, including its E-bit delay slot.
    # VU0 code has no version counter: compare this signature on every call.
    cull = [pairs0[pc] for pc in range(0xc0 // 8)]
    lines += ['AOT_WORDS(vu0_cull_words, 48, ' + ','.join(f'0x{w:08X}u' for pair in cull for w in pair) + ')']
    entries += fills('vu0_cull_words', data, [where0[pc] for pc in range(0xc0 // 8)],
                     [w for pair in cull for w in pair])
    basis = [pairs0[pc] for pc in range(0x5e8 // 8, 0x760 // 8)]
    lines += ['AOT_WORDS(vu0_basis_words, 94, ' + ','.join(f'0x{w:08X}u' for pair in basis for w in pair) + ')']
    entries += fills('vu0_basis_words', data, [where0[pc] for pc in range(0x5e8 // 8, 0x760 // 8)],
                     [w for pair in basis for w in pair])
    bounds = [pairs0[pc] for pc in range(0x2e0 // 8, 0x4f0 // 8)]
    lines += ['AOT_WORDS(vu0_bounds_words, 132, ' + ','.join(f'0x{w:08X}u' for pair in bounds for w in pair) + ')']
    entries += fills('vu0_bounds_words', data, [where0[pc] for pc in range(0x2e0 // 8, 0x4f0 // 8)],
                     [w for pair in bounds for w in pair])
    for name, start, end in (('length', 0x7b8, 0x808), ('direction', 0x808, 0x830)):
        native_pairs = [pairs0[pc] for pc in range(start // 8, end // 8)]
        words = [w for pair in native_pairs for w in pair]
        lines += [f'AOT_WORDS(vu0_{name}_words, {len(words)}, ' + ','.join(f'0x{w:08X}u' for w in words) + ')']
        entries += fills(f'vu0_{name}_words', data, [where0[pc] for pc in range(start // 8, end // 8)], words)
    tables0 = (blocks(pairs0), delay_slots(pairs0))
    found0 = {}
    for table in tables0:
        for pc, block in table.items():
            found0.setdefault(block, [where0[pc + k] for k in range(len(block))])
    unique0 = sorted(set(block for table in tables0 for block in table.values()))
    ids0 = {block: i for i, block in enumerate(unique0)}
    for i, block in enumerate(unique0):
        lines += emit_block('aot0', i, block, 0)
        entries += fills('aot0_words_%d' % i, data, found0[block], [w for pair in block for w in pair])
    lines += ['static const AotBlock *const aot0_blocks[2][512] = {  /* [delay slot][pc] */']
    for table in tables0:
        lines.append(' {')
        lines += [f'  &aot0_info_{ids0[table[pc]]},' if pc in table else '  NULL,' for pc in range(512)]
        lines.append(' },')
    lines += ['};',
              'static const AotBlock *aot0_find(unsigned pc, const uint32_t *words, unsigned remaining, int slot) {',
              '    const AotBlock *block = aot0_blocks[slot][pc & 511];',
              '    return block && block->count <= remaining && pc + block->count <= 512 &&',
              '           !memcmp(words, block->words, block->count * 8) ? block : NULL;', '}']
    lines += fill_function('aot_words', entries)
    text = '\n'.join(lines) + '\n'
    destination.parent.mkdir(parents=True, exist_ok=True)
    if not destination.exists() or destination.read_bytes() != text.encode():
        destination.write_bytes(text.encode())
    from vu1_native_clip import generate as generate_clip
    generate_clip(destination.parent / "vu1_native_clip.inc")
    return len(unique)


if __name__ == '__main__':
    print('VU1 AOT:', generate(ROOT / 'build/port/link/vu1_aot.inc'), 'straight-line blocks')
