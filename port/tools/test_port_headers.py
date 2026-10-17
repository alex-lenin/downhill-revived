"""Execute a shared-header virtual call using the retail SN vtable layout."""
from pathlib import Path
import subprocess
import transform

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build/port/runtime/headers'


def main():
    include = OUT / 'include'
    transform.port_headers(ROOT / 'include', include)
    header = (include / 'event_queue.h').read_text()
    assert 'virtual unsigned int Value' not in header and 'PS2_VCALL' in header
    assert 'virtual unsigned int Value' in (ROOT / 'include/event_queue.h').read_text()
    source = OUT / 'test.cpp'
    source.write_text('''#include "port.h"
#include "event_queue.h"
#include "menu.h"
static unsigned sounds;
extern "C" void func_001F7640(void) { sounds += 1; }
extern "C" void func_001F7680(void) { sounds += 2; }
typedef char size_check[sizeof(EventQueue) == 0x600 ? 1 : -1];
typedef char offset_check[__builtin_offsetof(EventQueue, ps2_vptr) == 0x5FC ? 1 : -1];
static unsigned char object[0x620] __attribute__((aligned(16)));
static unsigned int value(void *self) { return self == object + 28 ? 0x12345678 : 0; }
static unsigned int random(void *self) { return self == object + 12 ? 0x87654321 : 0; }
int main(void) {
 PlayMenuSelect(); PlayMenuBackLegacy(object);
 ps2_vslot table[7] = {};
 table[6].delta = 12; table[6].function = (void *)value;
 table[5].delta = -4; table[5].function = (void *)random;
 EventQueue *queue = (EventQueue *)(object + 16);
 queue->ps2_vptr = table;
 return sounds != 3 || queue->Value() != 0x12345678 || queue->Random() != 0x87654321;
}
''')
    obj = source.with_suffix('.o')
    subprocess.run(['clang', '--target=i686-pc-windows-itanium', '-c', '-O2',
                    '-fno-exceptions', '-fno-rtti', '-malign-double',
                    '-I' + str(ROOT / 'port/include'), '-I' + str(include),
                    str(source), '-o', str(obj)], check=True)
    exe = source.with_suffix('.exe')
    subprocess.run(['clang', '--target=i686-pc-windows-msvc', '-fuse-ld=lld',
                    str(obj), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
    print('Shared-header virtual layout, slots, signed this adjustments pass')


if __name__ == '__main__':
    main()
