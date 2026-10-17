"""Names host addresses of dhd.exe (a crash report's) from the linker map
build/port/link/dhd.map: the symbol each address is in, and the offset.
usage: python port/tools/addr2sym.py ADDR...   (hex)"""
import bisect, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
MAP = os.path.join(ROOT, 'build', 'port', 'link', 'dhd.map')


def symbols():
    out = []
    for line in open(MAP):
        m = re.match(r'\s*000[1-9]:[0-9a-f]+\s+(\S+)\s+([0-9a-f]{16})\s+(\S+)', line)
        if m:
            out.append((int(m.group(2), 16), m.group(1), m.group(3)))
    return sorted(out)


def main():
    syms = symbols()
    for a in sys.argv[1:]:
        addr = int(a, 16)
        i = bisect.bisect_right(syms, (addr, '\x7f')) - 1
        if i < 0:
            print('%08X ?' % addr)
        else:
            base, name, obj = syms[i]
            print('%08X %s+0x%X  (%s)' % (addr, name, addr - base, obj))


if __name__ == '__main__':
    main()
