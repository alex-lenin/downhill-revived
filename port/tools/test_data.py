"""Data conversion regressions; fixtures contain only synthetic text and addresses."""
from pathlib import Path
import json
import re
import struct
from unittest.mock import patch

import data
import transform


def check(directory):
    # An image global may be the first item in a linkage/namespace block,
    # including nested blocks and an initialized array. Function locals stay.
    source = '''extern "C" {
int D_0029E160[2];
namespace fixture {
int D_0029E164 = 1;
void f() { int D_0029E164 = 2; }
}
}
'''
    converted = transform.extern_image_data(source, {'D_0029E160', 'D_0029E164'})
    assert 'extern int D_0029E160[2];' in converted, converted
    assert re.search(r'extern int D_0029E164\s*;', converted), converted
    assert 'void f() { int D_0029E164 = 2; }' in converted, converted
    assert converted.count('\n') == source.count('\n')
    assembly = Path(directory) / 'alias-asm'
    assembly.mkdir(exist_ok=True)
    body = ['addiu $sp, $sp, -0x10', 'sd $ra, 0x0($sp)', 'jal timer_target',
            'nop', 'ld $ra, 0x0($sp)', 'jr $ra', 'addiu $sp, $sp, 0x10']
    source = 'alabel D_00123458\n' + ''.join(
        '/* 000000 %08X 00000000 */ %s\n' % (0x123458 + i*4, op)
        for i, op in enumerate(body)) + 'endlabel func_00123450\n'
    fixture = assembly / 'func_00123450.s'
    fixture.write_text(source)
    with patch.object(data.recomp, 'ASM', str(assembly)):
        aliases = data.forwarding_aliases({'_D_00123458', '_D_00123460', '_D_00270000'})
        assert aliases == {'_D_00123458': '_timer_target'}, aliases
        fixture.write_text(source.replace('nop\n', 'addu $a0, $a1, $zero\n'))
        assert data.forwarding_aliases({'_D_00123458'}) == {}
    (Path(directory) / 'code-alias.s').write_text('\n'.join(data.alias_directives(aliases)) + '\n')
    path = Path(directory) / 'pointer-text.s'
    path.write_text('''dlabel D_00275728
    /* 000000 00275728 */ .asciz "hJ)"
    .align 2
    /* 000004 0027572C 804A2900 */ .word D_00294A80
    /* 000008 00275730 */ .asciz "hJ)"
''')
    out, refs = [], set()
    with patch.object(data, 'data_entries', return_value={}):
        data.convert(str(path), out, {'D_00275728': 0x275728}, refs)
    assert out[2:] == ['.long _D_00294A68', '.p2align 2', '.long _D_00294A80', '.asciz "hJ)"'], out
    assert refs == {'_D_00294A68', '_D_00294A80'}, refs
    for slot, target in data.ASCIZ_POINTERS.items():
        value = int(target[2:], 16).to_bytes(4, 'little')
        assert value[-1] == 0 and 0 not in value[:-1]
        path.write_text('/* 000000 %08X */ .asciz %s\n' % (slot, json.dumps(value[:-1].decode('ascii'))))
        out, refs = [], set()
        data.convert(str(path), out, {}, refs)
        assert out == ['.long _' + target] and refs == {'_' + target}, (out, refs)
    # A source-layout change must fail rather than silently replace real text.
    path.write_text('/* 000000 00275728 */ .asciz "abc"\n')
    try:
        data.convert(str(path), [], {}, set())
    except ValueError:
        pass
    else:
        raise AssertionError('changed pointer bytes accepted')
    for slot, target in data.FLOAT_POINTERS.items():
        value = int(target[2:], 16).to_bytes(4, 'little')
        literal = repr(struct.unpack('<f', value)[0])
        path.write_text('/* 000000 %08X %s */ .float %s\n' % (slot, value.hex().upper(), literal))
        out, refs = [], set()
        data.convert(str(path), out, {}, refs)
        assert out == ['.long _' + target] and refs == {'_' + target}
    # The same subnormal outside the audited slots is still a float.
    path.write_text('/* 000000 00270000 30BD2700 */ .float 3.649452037e-39\n')
    out = []
    data.convert(str(path), out, {}, set())
    assert out == ['.float 3.649452037e-39']
    path.write_text('/* 000000 0027C414 30BD2700 */ .float 1.0\n')
    try:
        data.convert(str(path), [], {}, set())
    except ValueError:
        pass
    else:
        raise AssertionError('changed float pointer bytes accepted')
    print('Data pointer/text/float conversion tests passed')
