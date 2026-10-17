/* Test-only host adapter. Instruction execution and pipeline tracking come from
   pinned PCSX2 sources generated under build/ by test_vu1_pcsx2.py. */
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
#define INTC_VU1 0
static void hwIntcIrq(int) { std::abort(); }
struct { u64 cycle; } cpuRegs;
struct Logger { template<class... T> void Error(const char*, T...) { std::abort(); } } DbgCon;
struct VIFregisters { u32 top, itop; struct { bool VEW, VGW; } stat; } vif0Regs, vif1Regs;
struct { VIFregisters vifRegs; } vu1Thread;
#include "VU.h"
#include "VUops.h"
#include "VUflags.h"
VURegs vuRegs[2];
static std::vector<u8> oracle_gif;
enum { GIF_PATH_1, GIF_TRANS_XGKICK };
struct GifSink {
    u32 GetGSPacketSize(int, const u8 *mem, u32 address, u32, bool) {
        u64 tag; std::memcpy(&tag, mem + address, 8);
        unsigned loops = tag & 0x7fff, mode = tag >> 58 & 3, regs = tag >> 60;
        if (!regs) regs = 16;
        unsigned qwords = mode >= 2 ? loops : mode == 0 ? loops * regs : (loops * regs + 1) / 2;
        if (qwords >= 4095) std::abort();
        return (qwords + 1) * 16 | ((tag & 0x8000) ? 0x80000000u : 0);
    }
    void TransferGSPacketData(int, const u8 *data, unsigned size, bool) {
        oracle_gif.insert(oracle_gif.end(), data, data + size);
        if (oracle_gif.size() > 16 * 1024 * 1024) std::abort();
    }
} gifUnit;
#include "VUflags.cpp"
#include "VUops.cpp"
#include "VU1microInterp.cpp"

static std::vector<u8> native_gif;
static ps2_q128 input_memory[1024], native_memory[1024], reference_memory[1024];
static u32 microcode[4096];
#ifdef PS2_VU_AOT_TEST
extern "C" int ps2_vu1_reference(ps2_cpu *, unsigned, unsigned, unsigned, ps2_vu_kick_fn, void *);
#endif
static int native_kick(void *, unsigned address)
{
    for (unsigned tags = 0; tags < 4096; tags++) {
        unsigned at = address & 1023;
        const u8 *memory = reinterpret_cast<const u8*>(native_memory);
        u32 size = gifUnit.GetGSPacketSize(0, memory, at * 16, 0, false);
        unsigned qwords = (size & 65535) / 16;
        for (unsigned i = 0; i < qwords; i++) {
            const u8 *p = memory + ((address++ & 1023) * 16);
            native_gif.insert(native_gif.end(), p, p + 16);
        }
        if (size >> 31) return 0;
    }
    return -1;
}

static int compare(const ps2_cpu& input, unsigned address, unsigned top, unsigned itop)
{
    ps2_cpu native = input;
    memcpy(native_memory, input_memory, sizeof input_memory);
    memcpy(reference_memory, input_memory, sizeof input_memory);
    native.vu0_code = reinterpret_cast<u8*>(microcode); native.vu0_mem = native_memory;
    native_gif.clear(); oracle_gif.clear();
    VU0 = VURegs{}; VU1 = VURegs{}; VU1.idx = 1;
    VU1.Micro = reinterpret_cast<u8*>(microcode); VU1.Mem = reinterpret_cast<u8*>(reference_memory);
    memcpy(VU1.VF, input.vf, sizeof input.vf);
    VU1.VF[0] = {}; VU1.VF[0].f.w = 1;
    memcpy(&VU1.ACC, input.vacc, sizeof input.vacc);
    for (unsigned i = 0; i < 16; i++) VU1.VI[i].UL = input.vi[i];
    VU1.VI[0].UL = 0;
    VU1.VI[REG_R].UL = input.r; VU1.VI[REG_I].F = input.i;
    VU1.VI[REG_Q].F = VU1.q.F = input.q; VU1.VI[REG_P].F = VU1.p.F = input.p;
    VU1.VI[REG_STATUS_FLAG].UL = VU1.statusflag = input.vu0_ctrl[0];
    VU1.VI[REG_MAC_FLAG].UL = VU1.macflag = input.vu0_ctrl[1];
    VU1.VI[REG_CLIP_FLAG].UL = VU1.clipflag = input.vu0_ctrl[2];
    VU1.VI[REG_TPC].UL = address; VU0.VI[REG_VPU_STAT].UL = 0x100;
    vif1Regs.top = top; vif1Regs.itop = itop;
    unsigned old_mxcsr = _mm_getcsr();
    // PCSX2's normal VU configuration: toward zero, denormals flushed.
    _mm_setcsr((old_mxcsr & ~0x6000) | 0xE040);
    unsigned steps = 0;
    while ((VU0.VI[REG_VPU_STAT].UL & 0x100) && steps++ < (1u << 20)) {
        VU1.VI[REG_TPC].UL &= 0x3fff;
        unsigned trace_pc = VU1.VI[REG_TPC].UL;
        vu1Exec(&VU1);
        if (getenv("DHD_VU_TRACE")) {
            fprintf(stderr, "R %04X %llu", trace_pc, VU1.cycle);
            for (unsigned v = 1; v < 32; v++) for (unsigned c = 0; c < 4; c++) fprintf(stderr, " %08X", VU1.VF[v].UL[c]);
            fprintf(stderr, "\n");
        }
    }
    _mm_setcsr(old_mxcsr);
    if (VU0.VI[REG_VPU_STAT].UL & 0x100) { printf("reference timeout at %04X\n", VU1.VI[REG_TPC].UL); return 2; }
    int result = ps2_vu1_run(&native, address, top, itop, native_kick, nullptr);
    if (result) { printf("native failed %d\n", result); return 1; }
    unsigned differences = 0;
    auto check = [&](const char *name, unsigned i, u32 got, u32 expected) {
        if (got != expected) {
            if (differences < 12) printf("%s[%u] native=%08X PCSX2=%08X\n", name, i, got, expected);
            differences++;
        }
    };
    for (unsigned i = 1; i < 32; i++) for (unsigned j = 0; j < 4; j++) check("vf",i*4+j,native.vf[i].w[j],VU1.VF[i].UL[j]);
    for (unsigned i = 1; i < 16; i++) check("vi",i,native.vi[i],VU1.VI[i].US[0]);
    for (unsigned i = 0; i < 4; i++) check("acc",i,std::bit_cast<u32>(native.vacc[i]),VU1.ACC.UL[i]);
    check("Q",0,std::bit_cast<u32>(native.q),VU1.VI[REG_Q].UL);
    check("P",0,std::bit_cast<u32>(native.p),VU1.VI[REG_P].UL);
    check("I",0,std::bit_cast<u32>(native.i),VU1.VI[REG_I].UL); check("R",0,native.r,VU1.VI[REG_R].UL);
    for (unsigned i = 0; i < 3; i++) check("flags",i,native.vu0_ctrl[i],VU1.VI[16+i].UL);
    check("TPC",0,native.vu0_ctrl[10],VU1.VI[REG_TPC].UL & 0x3fff);
    for (unsigned i = 0; i < 4096; i++) check("memory",i,reinterpret_cast<u32*>(native_memory)[i],reinterpret_cast<u32*>(reference_memory)[i]);
    if (native_gif != oracle_gif) { printf("GIF differs: native %zu bytes, PCSX2 %zu bytes\n", native_gif.size(), oracle_gif.size()); differences++; }
#ifdef PS2_VU_AOT_TEST
    // Run the unchanged interpreter from the identical input. The kick sink
    // uses native_memory, so save translated results before reusing it.
    ps2_cpu interpreted = input;
    std::vector<u8> translated_memory(reinterpret_cast<u8*>(native_memory),
                                     reinterpret_cast<u8*>(native_memory) + sizeof native_memory);
    std::vector<u8> translated_gif = native_gif;
    memcpy(native_memory, input_memory, sizeof input_memory);
    native_gif.clear();
    interpreted.vu0_code = reinterpret_cast<u8*>(microcode); interpreted.vu0_mem = native_memory;
    if (ps2_vu1_reference(&interpreted, address, top, itop, native_kick, nullptr) ||
        memcmp(&interpreted, &native, sizeof native) ||
        memcmp(native_memory, translated_memory.data(), sizeof native_memory) ||
        native_gif != translated_gif) {
        printf("AOT differs from reference interpreter\n"); differences++;
    }
#endif
    if (const char *prefix = getenv("DHD_ORACLE_DUMP")) {
        char path[1024];
        snprintf(path, sizeof path, "%s.gif", prefix);
        if (FILE *f = fopen(path, "wb")) { fwrite(native_gif.data(), 1, native_gif.size(), f); fclose(f); }
        snprintf(path, sizeof path, "%s.mem", prefix);
        if (FILE *f = fopen(path, "wb")) { fwrite(native_memory, sizeof native_memory, 1, f); fclose(f); }
    }
    printf("entry %04X: %u reference steps, %zu GIF bytes, %u differences\n", address, steps, native_gif.size(), differences);
    return differences != 0;
}

int main(int argc, char **argv)
{
    if ((argc == 5 || argc == 6) && !strcmp(argv[1], "--entry")) {
        FILE *f = fopen(argv[2], "rb");
        if (!f || fread(microcode, sizeof microcode, 1, f) != 1) return 2;
        fclose(f);
        ps2_cpu input = {};
        unsigned seed = (unsigned)strtoul(argv[4], nullptr, 0);
        for (unsigned i = 0; i < 4096; i++) reinterpret_cast<u32*>(input_memory)[i] = 0x3f800001;
        for (unsigned i = 1; i < 32; i++) for (unsigned j = 0; j < 4; j++) input.vf[i].w[j] = 0x3f800001;
        input.q = input.p = 1; input.r = 0x3f800000;
        input.vu0_code = reinterpret_cast<u8*>(microcode); input.vu0_mem = input_memory;
        unsigned entry = (unsigned)strtoul(argv[3], nullptr, 16);
        if (seed == 1) memset(input_memory, 0, sizeof input_memory);
        if (entry == 0x17c8) {
            input_memory[8].w[3] = 2; input_memory[9].w[3] = 0;
            input_memory[10].w[3] = 0;
        }
        if (entry == 0x50 && microcode[20] == 0x1008006c) {
            memset(&input_memory[108], 0, 16); input_memory[108].w[0] = 0x8000;
        }
        if (seed >= 2) {
            unsigned random = seed;
            for (unsigned i = 0; i < 4096; i++) {
                random = random * 1664525u + 1013904223u;
                // Keep low 16-bit counts/indices at one while varying float mantissas.
                reinterpret_cast<u32*>(input_memory)[i] = 0x3f800001 | ((random >> 9 & 127) << 16);
            }
            if (entry == 0x17c8) {
                input_memory[8].w[3] = 2; input_memory[9].w[3] = input_memory[10].w[3] = 0;
            }
            if (entry == 0x50 && microcode[20] == 0x1008006c) {
                memset(&input_memory[108], 0, 16); input_memory[108].w[0] = 0x8000;
            }
        }
        if (entry == 0x850) {
            // This path preserves its supplied GIF header at qword 8.
            input_memory[8].w[0] = 0x8001;
            input_memory[8].w[1] = 0x30000000;
            input_memory[8].w[2] = 0x512;
        }
        if (argc == 6) {
            memset(&input_memory[996], 0, 16); input_memory[996].f[3] = 10000;
        }
        if (compare(input, 0, 0, 0)) return 1;
        ps2_vu1_run(&input, 0, 0, 0, nullptr, nullptr);
        int result = compare(input, entry, 0, 0);
        if (result || argc == 5) return result;
        ps2_vu1_run(&input, entry, 0, 0, nullptr, nullptr);
        unsigned continuation = input.vu0_ctrl[10];
        for (unsigned q = 568; q < 640; q++) memset(&input_memory[q], 0, 16);
        input_memory[568].w[0] = (entry == 0x2738 || entry == 0x28d0) ? 0x8004 : 0x8003;
        input_memory[568].w[1] = 0x30000000;
        input_memory[568].w[2] = 0x512;
        for (unsigned q = 570; q < 582; q += 3) {
            for (unsigned c = 0; c < 4; c++) input_memory[q].w[c] = 128;
            input_memory[q + 1].w[0] = 4096;
            input_memory[q + 1].w[1] = 4096;
            input_memory[q + 1].w[2] = 4096;
        }
        return compare(input, continuation, 0, 0);
    }
    if (argc != 2) { fprintf(stderr, "usage: oracle captured-vu-input.bin\n"); return 2; }
    ps2_cpu input; unsigned header[4];
    FILE *f = fopen(argv[1], "rb");
    if (!f || fread(header, sizeof header, 1, f) != 1 || header[3] != sizeof input ||
        fread(&input, sizeof input, 1, f) != 1 || fread(microcode, sizeof microcode, 1, f) != 1 ||
        fread(input_memory, sizeof input_memory, 1, f) != 1) { fprintf(stderr,"bad capture\n"); return 2; }
    fclose(f);
    return compare(input, header[0], header[1], header[2]);
}
