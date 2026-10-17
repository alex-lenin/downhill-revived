/* UNIT 0x00143A80 0x00143B98 */
struct Box { float x0; float x4; float x8; float xC; };
struct Elem { char pad0[0x10]; struct Box x10[4]; int x50; char pad54[0x11C]; };
struct Mgr { char pad0[0x4]; int count; char pad8[0x38]; int cur; int i; char pad48[0x28]; struct Elem elems[1]; };

extern void func_001439A8(struct Elem *a, struct Elem *b, struct Box *box);

void func_00143A80(struct Mgr *m)
{
    struct Elem *cur;
    struct Elem *other;
    int j;
    float *f;

    other = m->elems;
    cur = &m->elems[m->cur];
    f = (float *)cur;
    for (j = 0; j < 4; j++) {
        f[4] = f[5] = 0.0f;
        f[6] = f[7] = 500000.0f;
        f += 4;
    }
    for (m->i = 0; m->i < m->count; m->i++) {
        if (m->cur != m->i) {
            if (cur->x50 >= 2 && cur->x50 != 5 && other->x50 >= 2) {
                func_001439A8(cur, other, cur->x10);
            }
        }
        other++;
    }
    m->cur++;
    if (m->cur == m->count) {
        m->cur = 0;
    }
}
