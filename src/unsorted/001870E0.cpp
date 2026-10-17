/* UNIT 0x001870E0 0x00187328 */
class Task {
public:
    int x0;
    virtual void reset();
    virtual void unused();
    virtual unsigned int ready(int);
    virtual void step(int);
};

struct Obj {
    char pad0[0x64];
    Task *x64[2];
    int x6C;
    int x70;
    int x74;
};

extern Obj D_003F52D0;
extern void func_001B5A38(Obj *);
extern Obj *func_001B5BA8(Obj *, Obj *);
extern void func_001B4DD0(Obj *);
extern void func_001B5130(Obj *);
extern void func_001B5AC8(Obj *);
extern void func_00187038(Obj *);

void func_001870E0(Obj *self, int mode)
{
    int found = 0;
    int i;
    int pass;
    int j;
    int k;
    int l;
    Task **task2;
    Task **task3;
    Task **task4;

    for (i = 0; i < self->x6C; i++)
        self->x64[i]->reset();
    pass = 0;
    func_001B5A38(self);
    func_001B5BA8(self, &D_003F52D0);
    do {
        func_001B4DD0(self);
        j = 0;
        if (self->x6C > 0) {
            task2 = self->x64;
            do {
                (*task2)->step(mode);
                j++;
                task2++;
            } while (j < self->x6C);
        }
        pass++;
        func_001B5130(self);
        self->x74++;
    } while (pass < 4);
    self->x74 = 0;
    while (!found) {
        func_001B4DD0(self);
        k = 0;
        if (self->x6C > 0) {
            task3 = self->x64;
            do {
                found = ((*task3)->ready(mode) != 0) | found;
                k++;
                task3++;
            } while (k < self->x6C);
        }
        func_001B5130(self);
        self->x74++;
    }
    while (self->x74 < self->x70) {
        func_001B4DD0(self);
        l = 0;
        if (self->x6C > 0) {
            task4 = self->x64;
            do {
                (*task4)->step(mode);
                l++;
                task4++;
            } while (l < self->x6C);
        }
        func_001B5130(self);
        self->x74++;
    }
    func_001B4DD0(self);
    func_001B5AC8(self);
    func_001B5BA8(&D_003F52D0, self);
    func_00187038(self);
}
