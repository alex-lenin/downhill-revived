/* UNIT 0x00152210 0x001522D0 */
typedef int s32;

void func_001528C0(void *, s32);
void func_00152D38(void *, s32);
void func_00153118(void *, s32);
void func_00153448(void *, s32);
void func_00153A30(void *, s32);

struct Sub {
    char pad[0x24];
    void *parent;
};

struct Obj {
    s32 x00;
    s32 pad04;
    s32 x08;
    struct Sub sub0C;
    char pad1[0x34 - (0x0C + sizeof(struct Sub))];
    struct Sub sub34;
    char pad2[0x5C - (0x34 + sizeof(struct Sub))];
    struct Sub sub5C;
    char pad3[0x84 - (0x5C + sizeof(struct Sub))];
    struct Sub sub84;
    char pad4[0xB0 - (0x84 + sizeof(struct Sub))];
    struct Sub subB0;
};

void func_00152210(struct Obj *arg0, s32 arg1) {
    struct Sub *s6 = &arg0->sub0C;
    struct Sub *s5 = &arg0->sub34;
    struct Sub *s4 = &arg0->sub5C;
    struct Sub *s3 = &arg0->sub84;
    struct Sub *s2 = &arg0->subB0;

    arg0->x00 = arg1;
    arg0->x08 = 0;

    func_001528C0(s6, arg1);
    func_00152D38(s5, arg1);
    func_00153118(s4, arg1);
    func_00153448(s3, arg1);
    func_00153A30(s2, arg1);

    s6->parent = arg0;
    s5->parent = arg0;
    s4->parent = arg0;
    s3->parent = arg0;
    s2->parent = arg0;
}
