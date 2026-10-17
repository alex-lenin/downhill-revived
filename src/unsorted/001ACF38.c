/* UNIT 0x001ACF38 0x001ACF90 */
struct Timer {
    char pad0[0x8];
    float x8;
    float xC;
    int x10;
    int x14;
};

extern float func_001ACE98(struct Timer *timer);
extern int func_00238F48(void);

void func_001ACF38(struct Timer *timer, int duration, float value)
{
    int now;

    timer->xC = func_001ACE98(timer);
    timer->x8 = value;
    now = func_00238F48();
    timer->x14 = now;
    timer->x10 = now + duration;
}
