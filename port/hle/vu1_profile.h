/* Opt-in VU1 dispatch timings. No guest state changes; GIF time is accounted
   separately so the selected native kernel is not credited for GS work. */
#ifndef DHD_VU1_PROFILE_H
#define DHD_VU1_PROFILE_H
extern volatile uint32_t gs_presented_frames;
static FILE *vu_profile_file;
static int vu_profile_initialized, vu_profile_active, vu_profile_by_field;
static unsigned vu_profile_step, vu_profile_field;
extern uint64_t kernel_vsync_count;
static double vu_profile_after, vu_profile_until, vu_profile_start, vu_profile_kick;
static unsigned vu_profile_frame;
static struct VuProfileRow {
    unsigned used, overlay, entry, address, continuation, calls;
    double total, kick;
} vu_profile_rows[256];

static int vu_profile_enabled(void)
{
    if (!vu_profile_initialized) {
        vu_profile_initialized = 1;
        const char *path = getenv("DHD_VU1_PROFILE");
        if (path) {
            vu_profile_file = fopen(path, "w");
            if (!vu_profile_file) { fprintf(stderr, "cannot open VU1 profile %s\n", path); return 0; }
            const char *after = getenv("DHD_VU1_PROFILE_AFTER"), *until = getenv("DHD_VU1_PROFILE_UNTIL");
            vu_profile_after = after ? atof(after) : 0;
            vu_profile_until = until ? atof(until) : 1e30;
            /* DHD_VU1_PROFILE_FIELDS=first-last: the window in virtual GS fields (as DHD_BENCH), one row
               per key for the whole window, instead of wall seconds. */
            /* DHD_VU1_PROFILE_FIELDS=first-last[/step] also writes the rows every step fields. */
            const char *fields = getenv("DHD_VU1_PROFILE_FIELDS");
            if (fields && sscanf(fields, "%lf-%lf/%u", &vu_profile_after, &vu_profile_until, &vu_profile_step) >= 2)
                vu_profile_by_field = 1;
            fprintf(vu_profile_file, "start,end,frames,overlay,entry,address,mscnt,calls,vu_ms,gif_ms\n");
        }
    }
    return vu_profile_file != NULL;
}

static void vu_profile_flush(double now)
{
    if (!vu_profile_start) return;
    unsigned frames = gs_presented_frames - vu_profile_frame;
    for (unsigned i = 0; i < 256; i++) {
        struct VuProfileRow *r = &vu_profile_rows[i];
        if (r->used) fprintf(vu_profile_file, "%.6f,%.6f,%u,%u,%04X,%04X,%u,%u,%.6f,%.6f\n",
            vu_profile_start, now, frames, r->overlay, r->entry, r->address,
            r->continuation, r->calls, (r->total - r->kick) * 1000, r->kick * 1000);
    }
    fflush(vu_profile_file);
    memset(vu_profile_rows, 0, sizeof vu_profile_rows);
    vu_profile_start = now; vu_profile_frame = gs_presented_frames;
}

static double vu_profile_begin(void)
{
    if (!vu_profile_enabled()) return 0;
    double now = host_wall_seconds(), at = vu_profile_by_field ? (double)kernel_vsync_count : now;
    if (at < vu_profile_after) return 0;
    if (at >= vu_profile_until) {
        vu_profile_flush(now); fclose(vu_profile_file); vu_profile_file = NULL; return 0;
    }
    if (!vu_profile_start) {
        vu_profile_start = now; vu_profile_frame = gs_presented_frames; vu_profile_field = (unsigned)at;
    }
    if (vu_profile_by_field ? vu_profile_step && at - vu_profile_field >= vu_profile_step : now - vu_profile_start >= 5) {
        vu_profile_flush(now); vu_profile_field = (unsigned)at;
    }
    vu_profile_active = 1; vu_profile_kick = 0;
    return host_wall_seconds();
}

static void vu_profile_end(double start, unsigned overlay, unsigned entry, unsigned addr, unsigned continuation)
{
    double elapsed = host_wall_seconds() - start;
    vu_profile_active = 0;
    unsigned at = ((entry >> 3) * 31 + (addr >> 3) * 17 + overlay * 7 + continuation) & 255;
    for (unsigned n = 0; n < 256; n++, at = (at + 1) & 255) {
        struct VuProfileRow *r = &vu_profile_rows[at];
        if (!r->used || (r->overlay == overlay && r->entry == entry && r->address == addr && r->continuation == continuation)) {
            r->used = 1; r->overlay = overlay; r->entry = entry; r->address = addr;
            r->continuation = continuation; r->calls++; r->total += elapsed; r->kick += vu_profile_kick;
            return;
        }
    }
    fprintf(stderr, "VU1 profile key table full\n");
    abort(); /* Never silently omit keys from the measured distribution. */
}
#endif
