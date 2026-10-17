/* UNIT 0x001ACE98 0x001ACF38 */
struct Timer {
    char pad0[0x8];
    float x8;
    float xC;
    int x10;
    int x14;
};

extern "C" int func_00238F48(void);

extern "C" float func_001ACE98(Timer *timer)
{
    float duration;
    float t;

    duration = (float)(timer->x10 - timer->x14);
    if (duration == 0.0f) {
        return 0.0f;
    }
    t = (float)(func_00238F48() - timer->x14) / duration;
    t = t <? 1.0f;
    t = t >? 0.0f;
    return (timer->x8 - timer->xC) * t + timer->xC;
}
