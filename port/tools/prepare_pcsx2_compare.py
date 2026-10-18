# SPDX-License-Identifier: GPL-3.0-or-later
"""Build an opt-in GPL PCSX2 adapter under build/; no game data is fetched.

The adapter reads a standard p2m2 file at g_FrameCount (zero-based boot fields),
and carries that count through the MTGS VSync packet to synchronous PNG capture.
No dialogs, injected keyboard input, or wall-clock screenshot requests are used.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import urllib.request

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'build/port/compare-pcsx2-src'
REVISION = 'fd9d310ccbb6b8b62c976da8886a3c8fd3a10ff3'
DEPS_URL = ('https://github.com/PCSX2/pcsx2-windows-dependencies/releases/download/'
            'latest-windows-dependencies/pcsx2-windows-dependencies.7z')
DEPS_SHA256 = 'a3133c7841e8fbdab6f4d1d5ad8efec31fb1ecda95ffed39f4d1f36bb7eb0007'

PAD_HOOK = r'''
// DHD comparison adapter, opt-in; GPL-3.0+ with the surrounding emulator.
// InputRecordingFile/PadData are the unmodified upstream format reader.
void DhdComparisonAddTarget(u32 completed);

// Race-relative mode (DHD_COMPARE_RACE), the same rule as the native port's
// race_sync_* (port/hle/autopad.c). Moments are keyed by the game's race clock
// (60 per second), which restarts at the countdown (menu 9, race phase 1) and at
// GO (phase 2, still running in phase 3). DHD_COMPARE_RACE_CAPTURE lists c+N /
// g+N clock values; each is captured on the vsync after its phase first shows a
// clock of at least N and on the 2 * DHD_COMPARE_RACE_WINDOW after it. From the
// countdown the boot movie is ignored: pads are neutral, then after GO the race
// movie is read at the clock value.
static bool DhdRaceSync()
{
    static bool initialized = false, enabled = false;
    static InputRecordingFile race;
    static bool race_open = false;
    static bool started[2] = {};
    static bool crossed[16] = {};
    if (!initialized) {
        initialized = true;
        enabled = std::getenv("DHD_COMPARE_RACE") != nullptr;
        if (const char* path = std::getenv("DHD_COMPARE_RACE_MOVIE"); enabled && path && *path) {
            race_open = race.openExisting(path);
            if (!race_open) { std::fprintf(stderr, "DHD_COMPARE_ERROR race movie open\n"); std::exit(29); }
        }
    }
    if (!enabled) return false;
    const s32 menu = *reinterpret_cast<const s32*>(eeMem->Main + 0x29DED8);
    const s32 phase = *reinterpret_cast<const s32*>(eeMem->Main + 0x43F348);
    const s32 clock = *reinterpret_cast<const s32*>(eeMem->Main + 0x43EEC8);
    const bool unlocked_survey = std::getenv("DHD_COMPARE_RACE_UNLOCKED") != nullptr;
    const bool active_menu = menu == 9 || (unlocked_survey && menu == 2);
    if (unlocked_survey && !started[0] &&
        (*reinterpret_cast<const u32*>(eeMem->Main + 0x4346B4) != 0x035FFFFF ||
         *reinterpret_cast<const u32*>(eeMem->Main + 0x2B6E68) != 0 ||
         *reinterpret_cast<const u32*>(eeMem->Main + 0x2B6E6C) != 0 ||
         *reinterpret_cast<const u32*>(eeMem->Main + 0x2B6E70) != 0))
        return false; // Let NAV finish the bootstrap and reload first.
    if (g_FrameCount % 600 == 0) {
        std::fprintf(stderr, "DHD_COMPARE_PROGRESS input_index=%u menu=%d phase=%d\n", g_FrameCount, menu, phase);
        std::fflush(stderr);
    }
    for (int which = 0; which < 2; ++which)
        if (!started[which] && active_menu && phase == which + 1) {
            started[which] = true;
            std::fprintf(stderr, "DHD_COMPARE_RACE anchor=%c field=%u clock=%d\n", which ? 'g' : 'c', g_FrameCount, clock);
        }
    const char* window = std::getenv("DHD_COMPARE_RACE_WINDOW");
    const int w = window ? std::atoi(window) : 0;
    const char* cursor = std::getenv("DHD_COMPARE_RACE_CAPTURE");
    for (int i = 0; cursor && *cursor && i < 16; ++i) {
        char* end = nullptr;
        const int which = *cursor == 'g';
        const long value = std::strtol(cursor + 1, &end, 10);
        const bool in = started[which] && active_menu && (which ? phase == 2 || phase == 3 : phase == 1);
        if (!crossed[i] && in && clock >= value) {
            crossed[i] = true;
            // This field's VSync packet is already queued; capture from the next.
            for (int delta = 0; delta <= 2 * w; ++delta)
                DhdComparisonAddTarget(g_FrameCount + 2 + delta);
            std::fprintf(stderr, "DHD_COMPARE_RACE moment=%c%+ld field=%u clock=%d phase=%d\n",
                         *cursor, value, g_FrameCount, clock, phase);
            // DHD_COMPARE_RACE_DUMP: EE RAM then the scratchpad at the moment, as the port's
            // DHD_RACE_DUMP (port/hle/autopad.c) does, for comparing game state.
            if (const char* dump = std::getenv("DHD_COMPARE_RACE_DUMP")) {
                char path[1024];
                std::snprintf(path, sizeof path, "%s/ee-%c%+ld.bin", dump, *cursor, value);
                if (std::FILE* f = std::fopen(path, "wb")) {
                    std::fwrite(eeMem->Main, 32u << 20, 1, f);
                    std::fwrite(eeMem->Scratch, 16u << 10, 1, f);
                    std::fclose(f);
                }
            }
            // DHD_COMPARE_RACE_GSDUMP: an uncompressed GS dump (gs-c+N.gs) of the next two
            // fields, for comparing GS input (vertex Z, registers) with the port's.
            if (const char* gsdump = std::getenv("DHD_COMPARE_RACE_GSDUMP")) {
                char path[1024];
                std::snprintf(path, sizeof path, "%s/gs-%c%+ld.png", gsdump, *cursor, value);
                std::string target(path);
                std::replace(target.begin(), target.end(), '/', '\\');  // OpenCFile opens \\?\ paths, which keep '/'
                MTGS::RunOnGSThread([target]() {
                    GSConfig.GSDumpCompression = GSDumpCompressionMethod::Uncompressed;
                    GSQueueSnapshot(target, 2);
                });
            }
        }
        cursor = *end ? end + 1 : end;
    }
    std::fflush(stderr);
    if (!started[0]) return false;
    if (const char* stop = std::getenv("DHD_COMPARE_RACE_STOP");
        stop && started[1] && (phase == 2 || phase == 3) && clock >= std::atol(stop))
        VMManager::SetState(VMState::Stopping);
    static const std::array<u8, 18> neutral = {0xff, 0xff, 128, 128, 128, 128};  // as write_movie
    static u16 previous = 0xffff;
    const bool racing = started[1] && active_menu && (phase == 2 || phase == 3);
    for (unsigned port = 0; port < 2; ++port) {
        std::optional<PadData> pad;
        if (race_open && racing && clock >= 0 && u32(clock) < race.getTotalFrames())
            pad = race.readPadData(u32(clock), port, 0);
        // Opt-in stopped-clock results/replay exploration. A short-lived file
        // contains a hexadecimal active-high PS2 button mask; absent/invalid
        // files leave the recorded movie unchanged. No window focus required.
        if (const char* live = std::getenv("DHD_COMPARE_LIVE_PAD"); live && port == 0) {
            if (std::FILE* f = std::fopen(live, "rb")) {
                unsigned mask = 0;
                if (std::fscanf(f, "%x", &mask) == 1 && mask <= 0xffff) {
                    auto data = neutral;
                    data[0] = u8(~mask);
                    data[1] = u8(~(mask >> 8));
                    // Same pressure-byte order as the upstream p2m2 format and
                    // compare_pcsx2.write_movie, not just compact digital flags.
                    static constexpr unsigned bits[] = {5, 7, 4, 6, 12, 13, 14, 15, 10, 11, 8, 9};
                    for (unsigned i = 0; i < 12; ++i)
                        data[6 + i] = (mask & (1u << bits[i])) ? 255 : 0;
                    pad.emplace(port, 0, data);
                }
                std::fclose(f);
            }
        }
        if (!pad) pad.emplace(port, 0, neutral);
        pad->OverrideActualController();
        if (port == 0) {
            const u16 flags = pad->m_compactPressFlagsGroupOne | (u16(pad->m_compactPressFlagsGroupTwo) << 8);
            if (flags != previous) {
                std::fprintf(stderr, "DHD_COMPARE_PAD field=%u buttons=%04X clock=%d race\n", g_FrameCount, flags ^ 0xffff, clock);
                previous = flags;
            }
        }
    }
    return true;
}

static void DhdComparisonInput()
{
    static InputRecordingFile movie;
    static bool initialized = false;
    static bool enabled = false;
    if (!initialized) {
        initialized = true;
        const char* path = std::getenv("DHD_COMPARE_MOVIE");
        if (const char* nav = std::getenv("DHD_COMPARE_NAV"); nav && *nav) {
            enabled = true;
            std::fprintf(stderr, "DHD_COMPARE_ADAPTER=2 nav %s\n", nav);
        } else if (path && *path) {
            enabled = movie.openExisting(path);
            if (!enabled) { std::fprintf(stderr, "DHD_COMPARE_ERROR movie open\n"); std::exit(21); }
            std::fprintf(stderr, "DHD_COMPARE_ADAPTER=2 movie fields=%u\n", movie.getTotalFrames());
        }
    }
    if (!enabled) return;
    // DHD_COMPARE_RACE_SEED: the race seed the port pins with DHD_RACE_SEED (hle_race_seed). The
    // race reads it from timer 0 (0x0016B910: jal func_00238F10; nop), so it follows menu timing;
    // once the game is loaded that call becomes lui/ori of the value, as the port's patch does.
    if (static const char* seed = std::getenv("DHD_COMPARE_RACE_SEED"); seed && *seed) {
        static bool pinned = false;
        u32* code = reinterpret_cast<u32*>(eeMem->Main + 0x16B910);
        if (!pinned && code[0] == 0x0C08E3C4 && code[1] == 0) {
            const u32 value = std::strtoul(seed, nullptr, 0);
            code[0] = 0x3C020000 | (value >> 16);  // lui v0, value >> 16
            code[1] = 0x34420000 | (value & 0xFFFF);  // ori v0, v0, value & 0xFFFF
            Cpu->Clear(0x16B910, 2);
            pinned = true;
            std::fprintf(stderr, "DHD_COMPARE_RACE_SEED %u field=%u\n", value, g_FrameCount);
        }
    }
    // DHD_COMPARE_FRAMELOG: the game's frame counters on every field, for frame pacing
    // (game frames per field): D_0029E10C counts main-loop frames (func_001B5130),
    // D_00663108 load-bar frames (func_00221608).
    static const bool frame_log = std::getenv("DHD_COMPARE_FRAMELOG") != nullptr;
    if (frame_log) {
        const u8* ee = eeMem->Main;
        std::fprintf(stderr, "DHD_COMPARE_FRAME field=%u frame=%d loadbar=%d menu=%d phase=%d clock=%d\n", g_FrameCount,
                     *reinterpret_cast<const s32*>(ee + 0x29E10C), *reinterpret_cast<const s32*>(ee + 0x663108),
                     *reinterpret_cast<const s32*>(ee + 0x29DED8), *reinterpret_cast<const s32*>(ee + 0x43F348),
                     *reinterpret_cast<const s32*>(ee + 0x43EEC8));
    }
    if (std::getenv("DHD_COMPARE_SWEEP_STATE") && g_FrameCount % 30 == 0) {
        const u8* ee = eeMem->Main;
        std::fprintf(stderr, "DHD_SWEEP_STATE field=%u unlocks=%08X track=%d finished=%u checkpoint=%d modal=%08X\n",
            g_FrameCount, *reinterpret_cast<const u32*>(ee + 0x4346B4),
            *reinterpret_cast<const s32*>(ee + 0x4B2840), ee[0x43EF18],
            *reinterpret_cast<const s32*>(ee + 0x2B6C50),
            *reinterpret_cast<const u32*>(ee + 0x3F5420));
    }
    if (DhdRaceSync()) return;
    // DHD_COMPARE_NAV: the menus by the screen shown (menu_nav.h, pasted above), the same rule as
    // the port's DHD_NAV. The pad bytes are laid out as compare_pcsx2.write_movie's.
    if (const char* nav = std::getenv("DHD_COMPARE_NAV"); nav && *nav) {
        static MenuNav state;
        static u16 previous = 0;
        const s32 menu = *reinterpret_cast<const s32*>(eeMem->Main + 0x29DED8);
        const s32 phase = *reinterpret_cast<const s32*>(eeMem->Main + 0x43F348);
        const int step = state.step;
        unsigned buttons = 0;
        if (!menu_nav(&state, nav, menu, phase, [](unsigned address) { return *reinterpret_cast<const unsigned*>(eeMem->Main + (address & 0x1FFFFFCu)); },
                      g_FrameCount, &buttons)) {
            std::fprintf(stderr, "DHD_COMPARE_ERROR nav script\n");
            std::exit(24);
        }
        if (state.step != step)
            std::fprintf(stderr, "DHD_COMPARE_NAV step=%d field=%u menu=%d phase=%d\n", state.step, g_FrameCount, menu, phase);
        static constexpr int pressures[12] = {5, 7, 4, 6, 12, 13, 14, 15, 10, 11, 8, 9};  // right left up down ...
        std::array<u8, 18> data = {u8(~buttons), u8(~buttons >> 8), 128, 128, 128, 128};
        for (int i = 0; i < 12; ++i)
            data[6 + i] = (buttons >> pressures[i]) & 1 ? 255 : 0;
        static const std::array<u8, 18> neutral = {0xff, 0xff, 128, 128, 128, 128};
        // Opt-in two-player survey: both connected pads confirm the same NAV
        // steps, as the port's host_pad_script already does. Default stays P1.
        static const bool both = []() {
            const char* value = std::getenv("DHD_COMPARE_NAV_PLAYERS");
            return value && std::atoi(value) == 2;
        }();
        for (unsigned port = 0; port < 2; ++port) {
            PadData pad(port, 0, (port == 0 || both) ? data : neutral);
            pad.OverrideActualController();
        }
        if (buttons != previous) {
            std::fprintf(stderr, "DHD_COMPARE_PAD field=%u buttons=%04X nav\n", g_FrameCount, buttons);
            previous = u16(buttons);
        }
        return;
    }
    if (g_FrameCount % 600 == 0) {
        std::fprintf(stderr, "DHD_COMPARE_PROGRESS input_index=%u\n", g_FrameCount);
        std::fflush(stderr);
    }
    if (g_FrameCount >= movie.getTotalFrames()) {
        std::fprintf(stderr, "DHD_COMPARE_ERROR movie exhausted field=%u\n", g_FrameCount);
        std::exit(22);
    }
    static u16 previous = 0xffff;
    for (unsigned port = 0; port < 2; ++port) {
        const auto pad = movie.readPadData(g_FrameCount, port, 0);
        if (!pad) { std::fprintf(stderr, "DHD_COMPARE_ERROR pad read\n"); std::exit(23); }
        pad->OverrideActualController();
        if (port == 0) {
            u16 flags = pad->m_compactPressFlagsGroupOne | (u16(pad->m_compactPressFlagsGroupTwo) << 8);
            if (flags != previous) {
                std::fprintf(stderr, "DHD_COMPARE_PAD field=%u buttons=%04X\n", g_FrameCount, flags ^ 0xffff);
                previous = flags;
            }
        }
    }
    const char* until = std::getenv("DHD_COMPARE_UNTIL");
    if (until && g_FrameCount + 1 >= std::strtoul(until, nullptr, 10))
        VMManager::SetState(VMState::Stopping);
}
'''

CAPTURE_HOOK = r'''
// DHD comparison adapter: only the GS thread reads this value. The CPU copied
// it into unused padding in the existing VSync ring packet, so it cannot race.
u32 dhd_comparison_gs_frame = 0;

// Fields to capture: DHD_COMPARE_FIELDS (fixed) plus race-relative targets the
// CPU thread adds when an anchor is seen, always well before those fields.
static std::atomic<u32> dhd_race_targets[256];
static std::atomic<u32> dhd_race_target_count{0};

void DhdComparisonAddTarget(u32 completed)
{
    const u32 index = dhd_race_target_count.load();
    if (index >= std::size(dhd_race_targets)) std::exit(30);
    dhd_race_targets[index].store(completed);
    dhd_race_target_count.store(index + 1);
}

bool DhdComparisonTarget(u32 completed)
{
    static std::vector<u32> targets;
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        const char* cursor = std::getenv("DHD_COMPARE_FIELDS");
        while (cursor && *cursor) {
            char* end = nullptr;
            targets.push_back(static_cast<u32>(std::strtoul(cursor, &end, 10)));
            if (end == cursor || (*end && *end != ',')) std::exit(24);
            cursor = *end ? end + 1 : end;
        }
    }
    if (std::binary_search(targets.begin(), targets.end(), completed)) return true;
    for (u32 i = 0, n = dhd_race_target_count.load(); i < n; ++i)
        if (dhd_race_targets[i].load() == completed) return true;
    return false;
}
'''

CAPTURE_BODY = r'''
    if (const char* directory = std::getenv("DHD_COMPARE_CAPTURE_GPU")) {
        const u32 completed = dhd_comparison_gs_frame + 1;
        if (completed <= 20 || completed % 600 == 0) {
            std::fprintf(stderr, "DHD_COMPARE_GS completed=%u blank=%u\n", completed, blank_frame);
            std::fflush(stderr);
        }
        if (!blank_frame && DhdComparisonTarget(completed)) {
            std::fprintf(stderr, "DHD_COMPARE_READBACK_BEGIN completed=%u\n", completed);
            std::fflush(stderr);
            GSTexture* texture = g_gs_device->GetCurrent();
            if (!texture) std::exit(25);
            const u32 width = texture->GetWidth(), height = texture->GetHeight();
            const std::string name = Path::Combine(directory, fmt::format("field-{:06d}.png", completed));
            if (!texture->Save(name)) std::exit(26);
            std::fprintf(stderr, "DHD_COMPARE_CAPTURE completed=%u input_index=%u parity=%u size=%ux%u\n",
                         completed, dhd_comparison_gs_frame, field, width, height);
            std::fflush(stderr);
        }
    }
'''

SOFTWARE_CAPTURE = r'''
        // Export PCSX2's completed CPU raster buffer, avoiding driver readback.
        if (const char* directory = std::getenv("DHD_COMPARE_CAPTURE")) {
            const unsigned circuit = m_regs->PMODE.EN1 ? 0 : 1;
            const u32 completed = dhd_comparison_gs_frame + 1;
            if (unsigned(index) == circuit && DhdComparisonTarget(completed)) {
                const auto display = m_regs->DISP[circuit].DISPLAY;
                const auto fb = m_regs->DISP[circuit].DISPFB;
                const u32 width = (display.DW + 1) / (display.MAGH + 1);
                const u32 height = (display.DH + 1) / (display.MAGV + 1);
                const bool half = m_regs->SMODE2.INT && m_regs->SMODE2.FFMD;
                const int origin_x = int(fb.DBX) - off_x;
                const int origin_y = int(fb.DBY) - off_y;
                if (!width || !height || origin_x < 0 || origin_y < 0 || origin_x + int(width) > w ||
                    origin_y + int((height + unsigned(half)) >> unsigned(half)) > h) std::exit(27);
                std::vector<u32> pixels(width * height);
                for (u32 y = 0; y < height; ++y) {
                    const u32* row = reinterpret_cast<const u32*>(m_output + (origin_y + (y >> unsigned(half))) * pitch);
                    std::copy_n(row + origin_x, width, pixels.data() + y * width);
                }
                const std::string name = Path::Combine(directory, fmt::format("field-{:06d}.png", completed));
                if (!GSPng::Save(GSPng::RGB_PNG, name, reinterpret_cast<const u8*>(pixels.data()),
                                 width, height, width * 4, 1, false)) std::exit(28);
                std::fprintf(stderr, "DHD_COMPARE_CAPTURE completed=%u input_index=%u parity=%u size=%ux%u\n",
                             completed, dhd_comparison_gs_frame, unsigned(m_regs->CSR.rFIELD), width, height);
                std::fflush(stderr);
            }
        }
'''


def replace_once(path, old, new):
    text = path.read_text(encoding='utf-8')
    if new in text:
        return
    if text.count(old) != 1:
        raise RuntimeError(f'upstream anchor mismatch: {path}, {old[:80]!r}')
    path.write_text(text.replace(old, new), encoding='utf-8', newline='\n')


def patch_source(source=SOURCE):
    # Reset only this adapter's four downloaded upstream files, never a user's
    # project file. Reapplying produces identical source even if hooks changed.
    for relative in ('pcsx2/VMManager.cpp', 'pcsx2/MTGS.cpp',
                     'pcsx2/GS/Renderers/Common/GSRenderer.cpp', 'pcsx2/GS/GSCapture.cpp',
                     'pcsx2/GS/Renderers/SW/GSRendererSW.cpp'):
        original = subprocess.check_output(['git', '-C', str(source), 'show', REVISION + ':' + relative])
        (source / relative).write_bytes(original)
    vm = source / 'pcsx2/VMManager.cpp'
    replace_once(vm, '#include "Recording/InputRecording.h"',
                 '#include "Recording/InputRecording.h"\n#include "Recording/InputRecordingFile.h"\n#include "GS/GS.h"\n#include <algorithm>\n#include <cstdlib>\n#include <cstdio>')
    nav = (ROOT / 'port/hle/menu_nav.h').read_text(encoding='utf-8')  # shared with the port's DHD_NAV
    replace_once(vm, 'void VMManager::Internal::PollInputOnCPUThread()\n{',
                 nav + PAD_HOOK + '\nvoid VMManager::Internal::PollInputOnCPUThread()\n{')
    replace_once(vm, '\t\tg_InputRecording.handleControllerDataUpdate();\n\t}\n}',
                 '\t\tg_InputRecording.handleControllerDataUpdate();\n\t}\n\tDhdComparisonInput();\n}')
    mtgs = source / 'pcsx2/MTGS.cpp'
    replace_once(mtgs, '#include "VMManager.h"', '#include "VMManager.h"\n#include "Counters.h"\nextern u32 dhd_comparison_gs_frame;')
    replace_once(mtgs, 'remainder[4] = static_cast<u32>(registers_written);',
                 'remainder[4] = static_cast<u32>(registers_written);\n\tremainder[5] = g_FrameCount;')
    needle = 'GSvsync((((u32&)RingBuffer.Regs[0x1000]) & 0x2000) ? 0 : 1, remainder[4] != 0);'
    replace_once(mtgs, needle, 'dhd_comparison_gs_frame = remainder[5];\n\t\t\t\t\t\t\t' + needle)
    gs = source / 'pcsx2/GS/Renderers/Common/GSRenderer.cpp'
    replace_once(gs, '#include <algorithm>', '#include <algorithm>\n#include <atomic>\n#include <cstdlib>\n#include <cstdio>\n' + CAPTURE_HOOK)
    # Match upstream's screenshot point AFTER EndPresentFrame. A D3D12 readback
    # directly after Merge can wait on an unsubmitted command list indefinitely.
    needle = '\t// snapshot\n'
    replace_once(gs, needle, CAPTURE_BODY + '\n' + needle)
    sw = source / 'pcsx2/GS/Renderers/SW/GSRendererSW.cpp'
    replace_once(sw, '#include "GS/Renderers/SW/GSRendererSW.h"',
                 '#include "GS/Renderers/SW/GSRendererSW.h"\n#include "GS/GSPng.h"\n#include "common/Path.h"\n#include <algorithm>\n#include <cstdlib>\n#include <cstdio>\nextern u32 dhd_comparison_gs_frame;\nbool DhdComparisonTarget(u32 completed);')
    needle = '\t\tm_texture[index]->Update(out_r, m_output, pitch);'
    replace_once(sw, needle, needle + '\n' + SOFTWARE_CAPTURE)
    # Current public dependency bundle uses FFmpeg 8+, which removed AVCodec's
    # format arrays. Keep video capture compiling through its supported query API.
    capture = source / 'pcsx2/GS/GSCapture.cpp'
    version_header = source / 'deps/include/libavcodec/version_major.h'
    major = int(re.search(r'#define LIBAVCODEC_VERSION_MAJOR\s+(\d+)', version_header.read_text())[1]) if version_header.exists() else 0
    if major >= 62 and 'const AVPixelFormat* video_formats' not in capture.read_text():
        replace_once(capture, '\tX(avcodec_get_hw_config) \\',
                     '\tX(avcodec_get_hw_config) \\\n\tX(avcodec_get_supported_config) \\')
        replace_once(capture, '\t\tif (vcodec->pix_fmts)',
                     '\t\tconst AVPixelFormat* video_formats = nullptr;\n'
                     '\t\twrap_avcodec_get_supported_config(nullptr, vcodec, AV_CODEC_CONFIG_PIX_FORMAT, 0, reinterpret_cast<const void**>(&video_formats), nullptr);\n'
                     '\t\tif (vcodec->pix_fmts)')
        replace_once(capture, '\t\tbool supports_format = false;',
                     '\t\tconst AVSampleFormat* audio_formats = nullptr;\n'
                     '\t\twrap_avcodec_get_supported_config(nullptr, acodec, AV_CODEC_CONFIG_SAMPLE_FORMAT, 0, reinterpret_cast<const void**>(&audio_formats), nullptr);\n'
                     '\t\tbool supports_format = false;')
        replace_once(capture, '\t// rawvideo doesn\'t have a list of formats.',
                     '\tconst AVPixelFormat* video_formats = nullptr;\n'
                     '\twrap_avcodec_get_supported_config(nullptr, v_codec, AV_CODEC_CONFIG_PIX_FORMAT, 0, reinterpret_cast<const void**>(&video_formats), nullptr);\n'
                     '\t// rawvideo doesn\'t have a list of formats.')
        text = capture.read_text(encoding='utf-8')
        text = text.replace('vcodec->pix_fmts', 'video_formats').replace('v_codec->pix_fmts', 'video_formats')
        text = text.replace('acodec->sample_fmts', 'audio_formats')
        text = text.replace('bool supports_format = false;', 'bool supports_format = audio_formats == nullptr;')
        text = text.replace('p = audio_formats; *p != AV_SAMPLE_FMT_NONE;',
                            'p = audio_formats; p && *p != AV_SAMPLE_FMT_NONE;')
        capture.write_text(text, encoding='utf-8', newline='\n')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--patch-only', action='store_true')
    args = parser.parse_args()
    SOURCE.parent.mkdir(parents=True, exist_ok=True)
    if not SOURCE.exists():
        subprocess.run(['git', 'clone', '--depth', '1', '--branch', 'v2.8.2',
                        'https://github.com/PCSX2/pcsx2.git', str(SOURCE)], check=True)
    revision = subprocess.check_output(['git', '-C', str(SOURCE), 'rev-parse', 'HEAD'], text=True).strip()
    if revision != REVISION:
        raise RuntimeError(f'PCSX2 source revision mismatch: {revision}')
    patch_source()
    if args.patch_only:
        print(SOURCE)
        return
    archive = ROOT / 'build/port/compare-research/deps.7z'
    if not (SOURCE / 'deps/include/QtCore').exists():
        archive.parent.mkdir(parents=True, exist_ok=True)
        if not archive.exists():
            with urllib.request.urlopen(DEPS_URL, timeout=60) as response, archive.open('wb') as output:
                shutil.copyfileobj(response, output)
        if hashlib.sha256(archive.read_bytes()).hexdigest() != DEPS_SHA256:
            raise RuntimeError('dependency archive changed; review and pin a compatible bundle')
        subprocess.run(['7z', 'x', str(archive), '-o' + str(SOURCE), '-y'], check=True)
    patch_source()
    vswhere = Path(r'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe')
    vs = Path(subprocess.check_output([str(vswhere), '-latest', '-property', 'installationPath'], text=True).strip())
    msbuild = vs / 'MSBuild/Current/Bin/MSBuild.exe'
    command = [str(msbuild), str(SOURCE / 'PCSX2_qt.sln'), '/t:pcsx2-qt', '/m:4',
               '/p:Configuration=Release AVX2', '/p:Platform=x64',
               '/p:EXEString=pcsx2-dhd-compare',
               '/p:PlatformToolset=v145', '/p:DefaultPlatformToolset=v145', '/v:minimal', '/nologo']
    print('Building PCSX2 comparison adapter', flush=True)
    subprocess.run(command, cwd=SOURCE, check=True)
    print(SOURCE / 'bin/pcsx2-dhd-compare.exe')


if __name__ == '__main__':
    main()
