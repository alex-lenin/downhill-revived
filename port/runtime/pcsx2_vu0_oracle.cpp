/* Test-only host adapter for VU0 micro mode. Instruction execution and pipeline tracking come
   from pinned PCSX2 sources generated under build/ by test_vu0_pcsx2.py. Inputs are local
   DHD_VU0_CAPTURE files: registers, VU0 code/data (4 KB each) and the VU1 VF/VI file VU0
   addresses at 0x4000. */
#include <cstdint>
#include <bit>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <xmmintrin.h>
#include "ps2asm.h"
using u8 = uint8_t; using s8 = int8_t; using u16 = uint16_t; using s16 = int16_t;
using u32 = uint32_t; using s32 = int32_t; using u64 = uint64_t; using s64 = int64_t; using uint = unsigned;
struct u128 { u64 data[2]; }; using s128 = u128;
#define __ri inline
#define __fi inline
#define VUM_LOG(...) ((void)0)
#define CPU_LOG(...) std::abort()
#define pxFail(...) std::abort()
#define IdebugUPPER(...) ((void)0)
#define IdebugLOWER(...) ((void)0)
#define CHECK_VU_OVERFLOW(unit) true
#define CHECK_VUADDSUBHACK false
#define THREAD_VU1 false
#define INSTANT_VU1 false
#define CPU_INT(...) ((void)0)
#define INTC_VU0 0
#define INTC_VU1 0
#define VU0_PROGMASK 0xfff
static void hwIntcIrq(int) { std::abort(); }
struct { u64 cycle; } cpuRegs;
struct Logger {
    template<class... T> void Error(const char*, T...) { std::abort(); }
    template<class... T> void Warning(const char*, T...) {}
} DbgCon, DevCon;
struct VIFregisters { u32 top, itop; struct { bool VEW, VGW; } stat; } vif0Regs, vif1Regs;
struct { VIFregisters vifRegs; } vu1Thread;
#include "VU.h"
#include "VUops.h"
#include "VUflags.h"
VURegs vuRegs[2];
enum { GIF_PATH_1, GIF_TRANS_XGKICK };
struct GifSink {
    u32 GetGSPacketSize(int, const u8 *, u32, u32, bool) { std::abort(); }
    void TransferGSPacketData(int, const u8 *, unsigned, bool) { std::abort(); }
} gifUnit;
#include "VUflags.cpp"
#include "VUops.cpp"
#include "VU0microInterp.cpp"

static u8 code[4096];
static ps2_q128 input_memory[256], native_memory[256], reference_memory[256];
static ps2_q128 vu1_file[64];
static ps2_cpu vu1_state;

/* The capture's VU1 register file as the runtime's ps2_cpu (inverse of ps2_vu0_vu1_file). */
static void vu1_from_file(const ps2_q128 *file)
{
    vu1_state = {};
    memcpy(vu1_state.vf, file, sizeof vu1_state.vf);
    for (unsigned i = 0; i < 16; i++) vu1_state.vi[i] = (u16)file[32 + i].w[0];
    for (unsigned i = 0; i < 16; i++) vu1_state.vu0_ctrl[i] = file[48 + i].w[0];
    vu1_state.r = file[52].w[0]; memcpy(&vu1_state.i, &file[53].w[0], 4);
    memcpy(&vu1_state.q, &file[54].w[0], 4); memcpy(&vu1_state.p, &file[55].w[0], 4);
}

int main(int argc, char **argv)
{
    if (argc != 2) { fprintf(stderr, "usage: vu0-oracle captured-vu0-input.bin\n"); return 2; }
    ps2_cpu input; unsigned header[4];
    FILE *f = fopen(argv[1], "rb");
    if (!f || fread(header, sizeof header, 1, f) != 1 || header[1] != sizeof input || header[2] != sizeof vu1_file ||
        fread(&input, sizeof input, 1, f) != 1 || fread(code, sizeof code, 1, f) != 1 ||
        fread(input_memory, sizeof input_memory, 1, f) != 1 || fread(vu1_file, sizeof vu1_file, 1, f) != 1) {
        fprintf(stderr, "bad capture\n"); return 2;
    }
    fclose(f);
    unsigned address = header[0];

    /* PCSX2 */
    memcpy(reference_memory, input_memory, sizeof input_memory);
    VU0 = VURegs{}; VU1 = VURegs{}; VU1.idx = 1;
    VU0.Micro = code; VU0.Mem = reinterpret_cast<u8*>(reference_memory);
    memcpy(VU1.VF, vu1_file, 32 * 16);
    for (unsigned i = 0; i < 32; i++) VU1.VI[i].UL = vu1_file[32 + i].w[0];
    memcpy(VU0.VF, input.vf, sizeof input.vf);
    VU0.VF[0] = {}; VU0.VF[0].f.w = 1;
    memcpy(&VU0.ACC, input.vacc, sizeof input.vacc);
    for (unsigned i = 0; i < 16; i++) VU0.VI[i].UL = input.vi[i];
    VU0.VI[0].UL = 0;
    VU0.VI[REG_R].UL = input.r; VU0.VI[REG_I].F = input.i;
    VU0.VI[REG_Q].F = VU0.q.F = input.q;
    VU0.VI[REG_STATUS_FLAG].UL = VU0.statusflag = input.vu0_ctrl[0];
    VU0.VI[REG_MAC_FLAG].UL = VU0.macflag = input.vu0_ctrl[1];
    VU0.VI[REG_CLIP_FLAG].UL = VU0.clipflag = input.vu0_ctrl[2];
    VU0.VI[REG_TPC].UL = address; VU0.VI[REG_VPU_STAT].UL = 0x1;
    unsigned old_mxcsr = _mm_getcsr();
    // PCSX2's default VU0 FPCR: toward zero, denormals flushed.
    _mm_setcsr((old_mxcsr & ~0x6000) | 0xE040);
    unsigned steps = 0;
    while ((VU0.VI[REG_VPU_STAT].UL & 0x1) && steps++ < (1u << 20)) {
        unsigned trace_pc = VU0.VI[REG_TPC].UL & VU0_PROGMASK;
        vu0Exec(&VU0);
        if (getenv("DHD_VU_TRACE")) {
            fprintf(stderr, "R %04X %llu", trace_pc, VU0.cycle);
            for (unsigned v = 1; v < 32; v++) for (unsigned c = 0; c < 4; c++) fprintf(stderr, " %08X", VU0.VF[v].UL[c]);
            for (unsigned v = 1; v < 16; v++) fprintf(stderr, " %04X", VU0.VI[v].US[0]);
            fprintf(stderr, "\n");
        }
    }
    _mm_setcsr(old_mxcsr);
    if (VU0.VI[REG_VPU_STAT].UL & 0x1) { printf("reference timeout at %04X\n", VU0.VI[REG_TPC].UL); return 2; }

    /* Runtime interpreter, under the EE's normal MXCSR (round to nearest) as in the game. */
    ps2_cpu native = input;
    memcpy(native_memory, input_memory, sizeof input_memory);
    native.vu0_code = code; native.vu0_mem = native_memory;
    vu1_from_file(vu1_file);
    ps2_vu1_regs = &vu1_state;
    int result = ps2_vu0_run(&native, address);
    if (result) { printf("native failed %d\n", result); return 1; }

    unsigned differences = 0;
    auto check = [&](const char *name, unsigned i, u32 got, u32 expected) {
        if (got != expected) {
            if (differences < 16) printf("%s[%u] native=%08X PCSX2=%08X\n", name, i, got, expected);
            differences++;
        }
    };
    for (unsigned i = 1; i < 32; i++) for (unsigned j = 0; j < 4; j++) check("vf", i*4+j, native.vf[i].w[j], VU0.VF[i].UL[j]);
    for (unsigned i = 1; i < 16; i++) check("vi", i, native.vi[i], VU0.VI[i].US[0]);
    for (unsigned i = 0; i < 4; i++) check("acc", i, std::bit_cast<u32>(native.vacc[i]), VU0.ACC.UL[i]);
    check("Q", 0, std::bit_cast<u32>(native.q), VU0.VI[REG_Q].UL);
    check("I", 0, std::bit_cast<u32>(native.i), VU0.VI[REG_I].UL); check("R", 0, native.r, VU0.VI[REG_R].UL);
    for (unsigned i = 0; i < 3; i++) check("flags", i, native.vu0_ctrl[i], VU0.VI[16+i].UL);
    for (unsigned i = 0; i < 1024; i++) check("memory", i, reinterpret_cast<u32*>(native_memory)[i], reinterpret_cast<u32*>(reference_memory)[i]);
    for (unsigned i = 1; i < 32; i++) for (unsigned j = 0; j < 4; j++) check("vu1.vf", i*4+j, vu1_state.vf[i].w[j], VU1.VF[i].UL[j]);
    printf("entry %04X: %u reference steps, %u differences\n", address, steps, differences);
    return differences != 0;
}
