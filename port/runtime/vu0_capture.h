/* The registers of a DHD_VU0_CAPTURE file (vu0_interp.c's capture()) for the programs that replay
   it on the 64-bit host (pcsx2_vu0_oracle.cpp, vu0_native_diff.c, vu0_aot_diff.c).

   A capture holds the ps2_cpu of the program that wrote it: the game core's (wasm32), whose two
   pointers are 4 bytes each, with no padding before them. The members up to r have the same
   offsets on every target and cop0, vu0_ctrl and p follow the pointers, contiguous; the pointers
   themselves are not part of the state (the code and data memories follow in the file). */
#ifndef VU0_CAPTURE_H
#define VU0_CAPTURE_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "ps2asm.h"

/* Reads the registers of the capture f is at (after its 16-byte header); size is header[1].
   1 on success, with vu0_code and vu0_mem NULL. */
static inline int vu0_capture_cpu(FILE *f, unsigned size, ps2_cpu *cpu)
{
    const size_t head = offsetof(ps2_cpu, r) + sizeof cpu->r, tail = offsetof(ps2_cpu, p) + sizeof cpu->p - offsetof(ps2_cpu, cop0);
    const size_t narrow = (head + 2 * 4 + tail + 7) & ~(size_t)7;  /* 4-byte pointers, 8-byte alignment */
    unsigned char raw[sizeof(ps2_cpu)];
    memset(cpu, 0, sizeof *cpu);
    if ((size != sizeof *cpu && size != narrow) || fread(raw, size, 1, f) != 1) return 0;
    const size_t pointers = size == narrow ? 2 * 4 : offsetof(ps2_cpu, cop0) - head;
    memcpy(cpu, raw, head);
    memcpy(&cpu->cop0, raw + head + pointers, tail);
    return 1;
}

#endif
