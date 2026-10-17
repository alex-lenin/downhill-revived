/* UNIT 0x00158520 0x00158610 */
struct Anim {
    int x0;
    int state;
    float time;
    signed char delay;
    signed char countD;
    signed char countE;
    signed char countF;
    signed char x10;
    char pad11;
    signed char limit12;
    signed char limit13;
};

void func_00158520(struct Anim *a, float dt)
{
    if (a->delay != 0) {
        a->delay--;
        return;
    }
    a->time += dt;
    switch (a->state) {
    case 0:
        a->x10 += 1;
        if (++a->countD >= a->limit12) {
            a->state = 1;
        }
        break;
    case 1:
        a->x10 += 2;
        if (++a->countE >= a->limit13) {
            a->state = 2;
        }
        break;
    case 2:
        a->state = 3;
        a->countF++;
        a->x10 += 4;
        break;
    }
}
