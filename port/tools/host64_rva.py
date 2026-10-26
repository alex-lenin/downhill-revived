"""Names the RVAs of a dhd64.exe trap or fault report (launcher.c host64_trap, guest_memory.c
report_fault) from the lld-link map that build_host64.py writes.

    python port/tools/host64_rva.py build/w7g/exe/dhd64.map run.log     # every "rva 0x..." in the log
    python port/tools/host64_rva.py build/w7g/exe/dhd64.map 0x1a2b3c 0x4d5e
"""
import bisect
import re
import sys
from pathlib import Path


def symbols(map_path):
    text = Path(map_path).read_text(errors='replace')
    base = int(re.search(r'Preferred load address is ([0-9a-fA-F]+)', text).group(1), 16)
    rows = sorted({(int(va, 16) - base, name) for name, va in
                   re.findall(r'^ \w{4}:\w{8}\s+(\S+)\s+([0-9a-fA-F]{16})\s', text, re.M)})
    return [r[0] for r in rows], [r[1] for r in rows]


def main():
    addresses, names = symbols(sys.argv[1])
    wanted = []
    for arg in sys.argv[2:]:
        if Path(arg).is_file():
            wanted += re.findall(r'rva (0x[0-9a-fA-F]+)', Path(arg).read_text(errors='replace'))
        else:
            wanted.append(arg)
    for rva in wanted:
        value = int(rva, 16)
        i = bisect.bisect_right(addresses, value) - 1
        print(f'{rva}: {names[i]}+0x{value - addresses[i]:x}' if i >= 0 else f'{rva}: ?')


if __name__ == '__main__':
    main()
