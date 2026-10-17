/* UNIT 0x00242448 0x002424C8 */
typedef struct State {
    int x0;
    char pad4[0x28];
    int x2C;
    char pad30[0x4];
    int x34;
    int x38;
    int x3C;
    int x40;
} State;

void func_00242448(State *s, int n)
{
    int left;
    int take;

    if (s->x0 == 0) {
        left = 40 - s->x2C;
        if (n < left) {
            take = n;
        } else {
            take = left;
        }
        s->x2C += take;
        if (s->x2C >= 40) {
            s->x0 = 1;
        }
        n -= take;
    }
    s->x34 = (s->x34 + n) % s->x3C;
    s->x38 += n;
    s->x40 += n;
}
