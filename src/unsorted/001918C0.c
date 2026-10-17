/* UNIT 0x001918C0 0x00191978 */
struct VEntry {
    short delta;
    short pad2;
    char *(*fn)(void *);
};

struct VTable {
    char pad0[0x20];
    struct VEntry x20;
};

struct Element {
    struct VTable *vtbl;
    int x4;
    char pad8[0x84 - 0x8];
    char x84[0xA0 - 0x84];
};

struct Panel {
    char pad0[0x10];
    struct Element elements[3];
    char padF0[0x214 - 0x1F0];
    int count;
    int x218;
};

extern void func_00191640(struct Panel *p);
extern void func_00191FC0(struct Element *e);
extern void func_001BCAD0(void *a, int b, char *c);

void func_001918C0(struct Panel *p) {
    int i;
    int x4;
    char *body;
    struct Element *e;
    char *dst;

    if (p->x218) {
        func_00191640(p);
        p->x218 = 0;
    } else {
        i = 0;
        if (p->count > 0) {
            e = &p->elements[0];
            dst = p->elements[0].x84;
            do {
                i++;
                func_00191FC0(e);
                x4 = e->x4;
                body = e->vtbl->x20.fn((char *)e + e->vtbl->x20.delta);
                e++;
                func_001BCAD0(dst, x4, body + 0x10);
                dst += sizeof(struct Element);
            } while (i < p->count);
        }
    }
}
