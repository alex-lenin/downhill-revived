"""The PC target, shared by port/tools/build.py, data.py and link.py.

32-bit Windows with the Itanium C++ ABI (clang's windows-itanium): structs and bitfields are laid
out as by gcc, as on the PS2 (the MSVC ABI packs bitfields differently and 149 units use them),
and C++ member functions use the normal C calling convention, not thiscall, so C callers and the
game's function tables can call them. Objects are COFF and link with lld-link against the MSVC
C runtime.

Win32 C symbols carry a leading underscore (func_00123456 is _func_00123456 in the objects);
asm labels and the converted data add it by hand (LABEL_PREFIX)."""

TARGET = 'i686-pc-windows-itanium'
# The asm runtime and other host code use the platform's headers: plain C, same C ABI.
HOST_TARGET = 'i686-pc-windows-msvc'
LABEL_PREFIX = '_'

# The emulated PS2 RAM (32 MB) sits at this host address, with the uncached mirrors at
# 0x20000000/0x30000000 + it (port/runtime/memory.c). Below 0x10000000, so a PS2 DMA address
# (pointer & 0x0FFFFFFF) is still the host pointer; the exe is linked at IMAGE_BASE, above it all.
PS2_RAM_BASE = 0x08000000
IMAGE_BASE = 0x40000000
