/* UNIT 0x001232D0 0x00123350 .rodata=0x288D60:0x288D80 */
typedef struct { char pad[0x44]; int x44; } Obj;
extern int func_00222BD8(int, int);
void func_001232D0(Obj *obj) {
    switch (func_00222BD8(0, 4)) {
    case 0: obj->x44 = 5; break;
    case 1: obj->x44 = 6; break;
    case 2: obj->x44 = 7; break;
    case 3: obj->x44 = 1; break;
    case 4: obj->x44 = 2; break;
    }
}
