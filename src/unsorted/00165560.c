/* UNIT 0x00165560 0x00165648 */
typedef union Model {
    unsigned long bits;
    struct {
        unsigned long pad0 : 57;
        unsigned long hidden : 1;
        unsigned long pad58 : 6;
    } f;
} Model;

typedef struct Obj {
    char pad0[0x50];
    char x50[0xB0];
    Model *model;
    char pad104[0x190 - 0x104];
    int state;
    char pad194[0x454 - 0x194];
    unsigned char x454 : 1;
} Obj;

extern char D_002AF2C0[];
extern int func_0011C0D0(void *, void *, int, float);
extern void func_00165B90(Obj *);
extern void func_00165850(Obj *);
extern void func_001657F8(Obj *);

void func_00165560(Obj *obj) {
    Model *model = obj->model;

    if (!(model->bits & (1L << 57)) && !func_0011C0D0(D_002AF2C0, obj->x50, 0, 8.0f)) {
        model->f.hidden = obj->x454;
    }
    if (obj->state == 1) {
        func_00165B90(obj);
    } else if (obj->state == 0) {
        func_00165850(obj);
    } else if (obj->state == 2) {
        func_001657F8(obj);
    }
}
