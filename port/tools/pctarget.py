"""The target of the game units' C, shared by data.py, recomp.py, transform.py and build.py.

build_wasm.py sets TARGET to wasm32 and LABEL_PREFIX to '' before it imports the others: the
values below are the 32-bit Windows target that abi_check.py's source audit and the native unit
tests (port/tools/test_*.py, build.py's objects) still compile for. The 32-bit game build itself is
retired (tag last-32bit-0.3.1).

32-bit Windows with the Itanium C++ ABI (clang's windows-itanium): structs and bitfields are laid
out as by gcc, as on the PS2 (the MSVC ABI packs bitfields differently and 149 units use them),
and C++ member functions use the normal C calling convention, not thiscall, so C callers and the
game's function tables can call them. Objects are COFF.

Win32 C symbols carry a leading underscore (func_00123456 is _func_00123456 in the objects);
asm labels and the converted data add it by hand (LABEL_PREFIX)."""

TARGET = 'i686-pc-windows-itanium'
# The asm runtime and other host code of the native tests use the platform's headers: plain C,
# same C ABI.
HOST_TARGET = 'i686-pc-windows-msvc'
LABEL_PREFIX = '_'

# The emulated PS2 RAM (32 MB) sits at this guest address, with the uncached mirrors at
# 0x20000000/0x30000000 + it (port/runtime/memory.c). Below 0x10000000, so a PS2 DMA address
# (pointer & 0x0FFFFFFF) is still the guest pointer.
PS2_RAM_BASE = 0x08000000
