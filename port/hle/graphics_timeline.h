/* DHD_TIMELINE=1 (or a CSV path): where each graphics-pipeline thread spends the fields of the
   DHD_BENCH window. Measures only waits (idle/blocked time) with the performance counter, so busy
   time per thread is the field's wall time minus its waits; nothing is timed when it is unset.
   - game (EE): waiting in graphics_wait for the VU worker, then for the GS worker, by call site
     (host return address, see the link map), and for room in the VU ring;
   - vu: idle (no packet queued) and blocked on a full GS ring;
   - gs: idle (no packet queued).
   bench_vsync calls graphics_timeline at every field of the window; at its end the medians and
   means go to stderr ("bench timeline:" lines) and, with a path, one CSV row per field. */
#if !defined(GRAPHICS_QUEUE_TEST) || defined(GS_QUEUE_TEST)
enum { TL_EE_VU, TL_EE_GS, TL_EE_FULL, TL_VU_IDLE, TL_VU_FULL, TL_GS_IDLE, TL_COUNT };
static const char *const tl_names[TL_COUNT] = {"ee_wait_vu", "ee_wait_gs", "ee_ring_full", "vu_idle", "vu_gs_full",
                                              "gs_idle"};
static int tl_on = -1;
static volatile LONG64 tl_ticks[TL_COUNT];
enum { TL_SITES = 32 };
static struct { void *site; unsigned calls, pending; LONG64 ticks; } tl_sites[TL_SITES];

static int timeline_enabled(void)
{
    if (tl_on < 0) tl_on = getenv("DHD_TIMELINE") != NULL;
    return tl_on;
}

static LONG64 tl_now(void)
{
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return t.QuadPart;
}

/* Each counter has one writing thread; the interlocked add keeps the 64-bit value whole for the
   EE's reads on this 32-bit host. */
static void tl_add(unsigned which, LONG64 start)
{
    if (start) InterlockedExchangeAdd64(&tl_ticks[which], tl_now() - start);
}

static LONG64 tl_begin(void) { return timeline_enabled() ? tl_now() : 0; }

static void tl_site(void *site, int pending, LONG64 ticks)
{
    unsigned i = 0;
    while (i < TL_SITES - 1 && tl_sites[i].site && tl_sites[i].site != site) i++;
    tl_sites[i].site = site;
    tl_sites[i].calls++;
    tl_sites[i].pending += pending;
    tl_sites[i].ticks += ticks;
}

static int tl_compare(const void *a, const void *b)
{
    double x = *(const double *)a, y = *(const double *)b;
    return x < y ? -1 : x > y;
}

/* phase 0: the window's first field; 1: every later field; 2: the last one (report). */
void graphics_timeline(int phase, unsigned field, double wall_seconds)
{
    static LONG64 last[TL_COUNT];
    static double *rows;  /* per field: wall then TL_COUNT waits, in ms */
    static unsigned count, capacity, first;
    static double frequency;
    if (!timeline_enabled()) return;
    if (!frequency) {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        frequency = (double)f.QuadPart;
    }
    LONG64 now[TL_COUNT];
    for (unsigned i = 0; i < TL_COUNT; i++) now[i] = InterlockedCompareExchange64(&tl_ticks[i], 0, 0);
    if (phase == 0) {
        memcpy(last, now, sizeof last);
        memset(tl_sites, 0, sizeof tl_sites);
        first = field, count = 0;
        return;
    }
    if (count == capacity) {
        capacity = capacity ? capacity * 2 : 1024;
        rows = realloc(rows, capacity * (TL_COUNT + 1) * sizeof *rows);
        if (!rows) { tl_on = 0; return; }
    }
    double *row = rows + count++ * (TL_COUNT + 1);
    row[0] = wall_seconds * 1e3;
    for (unsigned i = 0; i < TL_COUNT; i++) row[i + 1] = (double)(now[i] - last[i]) * 1e3 / frequency;
    memcpy(last, now, sizeof last);
    if (phase != 2) return;
    /* Report: mean and median of each column, then the busy share of each thread. */
    double *column = malloc(count * sizeof *column), mean[TL_COUNT + 1], median[TL_COUNT + 1];
    if (!column) return;
    for (unsigned c = 0; c <= TL_COUNT; c++) {
        double sum = 0;
        for (unsigned r = 0; r < count; r++) sum += column[r] = rows[r * (TL_COUNT + 1) + c];
        qsort(column, count, sizeof *column, tl_compare);
        mean[c] = sum / count, median[c] = column[count / 2];
    }
    free(column);
    fprintf(stderr, "bench timeline: fields=%u mean_ms field=%.3f", count, mean[0]);
    for (unsigned i = 0; i < TL_COUNT; i++) fprintf(stderr, " %s=%.3f", tl_names[i], mean[i + 1]);
    fprintf(stderr, " | median_ms field=%.3f", median[0]);
    for (unsigned i = 0; i < TL_COUNT; i++) fprintf(stderr, " %s=%.3f", tl_names[i], median[i + 1]);
    fputc('\n', stderr);
    double field_ms = mean[0];
    fprintf(stderr, "bench timeline: busy_ms_per_field game=%.3f vu=%.3f gs=%.3f\n",
            field_ms - mean[1 + TL_EE_VU] - mean[1 + TL_EE_GS] - mean[1 + TL_EE_FULL],
            field_ms - mean[1 + TL_VU_IDLE] - mean[1 + TL_VU_FULL], field_ms - mean[1 + TL_GS_IDLE]);
    for (unsigned i = 0; i < TL_SITES && tl_sites[i].site; i++)
        fprintf(stderr, "bench timeline: ee_wait site=%p calls=%.2f pending=%.2f wait_ms=%.3f (per field)\n",
                tl_sites[i].site, (double)tl_sites[i].calls / count, (double)tl_sites[i].pending / count,
                (double)tl_sites[i].ticks * 1e3 / frequency / count);
    const char *path = getenv("DHD_TIMELINE");
    if (path && strcmp(path, "1")) {
        FILE *f = fopen(path, "w");
        if (f) {
            fprintf(f, "field,wall_ms");
            for (unsigned i = 0; i < TL_COUNT; i++) fprintf(f, ",%s", tl_names[i]);
            fputc('\n', f);
            for (unsigned r = 0; r < count; r++) {
                fprintf(f, "%u", first + 1 + r);
                for (unsigned c = 0; c <= TL_COUNT; c++) fprintf(f, ",%.4f", rows[r * (TL_COUNT + 1) + c]);
                fputc('\n', f);
            }
            fclose(f);
        }
    }
    fflush(stderr);
}
#else
static LONG64 tl_now(void) { return 0; }
static LONG64 tl_begin(void) { return 0; }
static void tl_add(unsigned which, LONG64 start) { (void)which, (void)start; }
static void tl_site(void *site, int pending, LONG64 ticks) { (void)site, (void)pending, (void)ticks; }
enum { TL_EE_VU, TL_EE_GS, TL_EE_FULL, TL_VU_IDLE, TL_VU_FULL, TL_GS_IDLE };
#endif
