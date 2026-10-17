/* UNIT 0x0015C690 0x0015C790 */
struct Child {
    char *x0;
    virtual void a();
    virtual void b();
    virtual int test();
};

struct Screen {
    char *x0;
    int x4;
    int x8;
};

extern int func_00238F48(void);
extern void func_0015BFF8(void *, void *);

void func_0015C690(Screen *obj)
{
    char *base = obj->x0;
    int elapsed = func_00238F48() - obj->x8;
    Child *current = (Child *)(base + 0x63E0);
    Child *dest = (Child *)(base + 0x63DC);
    Child *child;

    if (current->test()) {
        func_0015BFF8(dest, current);
    } else if (elapsed > 30) {
        child = (Child *)(base + 0x640C);
        if (child->test()) goto found;
        child = (Child *)(base + 0x6400);
        if (child->test()) {
found:
            func_0015BFF8(dest, child);
        } else {
            child = (Child *)(base + 0x63E8);
            if (child->test()) func_0015BFF8(dest, child);
        }
    }
}
