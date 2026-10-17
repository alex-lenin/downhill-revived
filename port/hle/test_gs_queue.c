#define WIN32_LEAN_AND_MEAN
#include "../gs/gs.h"
#include "gs_queue.h"
#include "hle.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
volatile uint32_t gs_presented_frames;
double vif_gs_seconds;
static uint32_t actual, expected, callbacks, failures;
static DWORD main_thread, consumer_thread;
void hle_log(const char *fmt, ...)
{
    (void)fmt;
}
void host_thread_name(const char *name) { (void)name; }
double host_wall_seconds(void)
{
    return 0;
}
void vif_feed(int unit, const uint32_t *words, uint32_t count)
{
    (void)unit;
    graphics_provenance.program = words[4];
    graphics_gif_feed(3, words, count);
}
static uint32_t hash(uint32_t old, uint32_t word)
{
    return (old ^ word) * 16777619u;
}
static void record(unsigned path, const uint32_t *words)
{
    DWORD thread = GetCurrentThreadId();
    if (!consumer_thread)
        consumer_thread = thread;
    failures += consumer_thread != thread || thread == main_thread;
    actual = hash(actual, path);
    actual = hash(actual, gs.vu_program);
    for (unsigned i = 0; i < 4; i++)
        actual = hash(actual, words[i]);
    if (!(++callbacks % 37))
        Sleep(1);
}
static void expect(unsigned path, unsigned program, const uint32_t *words, unsigned count)
{
    for (unsigned n = 0; n < count; n += 4) {
        expected = hash(expected, path);
        expected = hash(expected, program);
        for (unsigned i = 0; i < 4; i++)
            expected = hash(expected, words[n + i]);
    }
}
int main(void)
{
    main_thread = GetCurrentThreadId();
    _putenv("DHD_MTVU=1");
    _putenv("DHD_GS_WORKER=1");
    gs_reset(&gs);
    gs.record_transfer = record;
    uint32_t packet[8] = {0x8001, 0x10000000, 14, 0, 0, 0, GS_RGBAQ, 0};
    for (unsigned n = 0; n < 3000; n++) {
        packet[4] = n;
        expect(3, n, packet, 8);
        graphics_feed(1, packet, 8);
        memset(packet + 4, 0, 8); /* queued bytes must survive producer reuse */
    }
    graphics_wait();
    uint32_t memory[4096] = {0};
    for (unsigned n = 0; n < 3000; n++) {
        packet[4] = n + 3000;
        memcpy(memory + 4092, packet, 16);
        memcpy(memory, packet + 4, 16);
        graphics_provenance.program = n + 3000;
        expect(1, n + 3000, packet, 8);
        failures += graphics_xgkick(memory, 1023) != 0;
        memset(memory, 0, sizeof memory);
    }
    graphics_wait();
    failures += actual != expected || callbacks != 12000;
    graphics_shutdown();
    printf("split GS FIFO, wrap, copy ownership, provenance and fences: %u failures\n", failures);
    return failures != 0;
}
