/* UNIT 0x0012D838 0x0012D860 */
typedef struct Link {
    char pad0[0x2848];
    void *x2848;
} Link;

typedef struct Owner {
    char pad0[0x7A60];
    Link *x7A60;
    Link *x7A64;
} Owner;

void func_0012D838(Owner *self, Link *a, Link *b)
{
    if (a != 0) {
        self->x7A60 = a;
        a->x2848 = &self->x7A60;
    }
    if (b != 0) {
        self->x7A64 = b;
        b->x2848 = &self->x7A64;
    }
}
