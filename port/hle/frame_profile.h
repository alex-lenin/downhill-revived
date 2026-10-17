#ifdef DHD_FRAME_PROFILE_BUILD
/* Optional main-thread wall-time breakdown at virtual-field boundaries. */
static FILE *frame_profile_file;
static int frame_profile_initialized, frame_profile_active;
static double frame_profile_after, frame_profile_until, frame_profile_start, frame_profile_last;
static double frame_profile_parts[7];
static unsigned frame_profile_first;
extern volatile uint32_t gs_presented_frames;
static void frame_profile_mark(unsigned part)
{
    if (!frame_profile_initialized) {
        frame_profile_initialized = 1;
        const char *path = getenv("DHD_FRAME_PROFILE");
        if (path) {
            frame_profile_file = fopen(path, "w");
            if (!frame_profile_file) { perror(path); abort(); }
            const char *a = getenv("DHD_VU1_PROFILE_AFTER"), *b = getenv("DHD_VU1_PROFILE_UNTIL");
            frame_profile_after = a ? atof(a) : 90;
            frame_profile_until = b ? atof(b) : 150;
        }
    }
    if (!frame_profile_file) return;
    double now = host_wall_seconds();
    if (!frame_profile_active) {
        if (part || now < frame_profile_after) return;
        frame_profile_active = 1; frame_profile_start = frame_profile_last = now;
        frame_profile_first = gs_presented_frames;
    }
    frame_profile_parts[part] += now-frame_profile_last;
    frame_profile_last = now;
    if (!part && now >= frame_profile_until) {
        fprintf(frame_profile_file, "wall_s,frames,between_s,pace_s,wait1_s,handlers_s,wait2_s,present_s,end_handlers_s\n");
        fprintf(frame_profile_file, "%.6f,%u", now-frame_profile_start, gs_presented_frames-frame_profile_first);
        for (unsigned i=0;i<7;i++) fprintf(frame_profile_file, ",%.6f", frame_profile_parts[i]);
        fputc('\n',frame_profile_file);fclose(frame_profile_file);frame_profile_file=NULL;
    }
}

#else
#define frame_profile_mark(part) ((void)0)
#endif
