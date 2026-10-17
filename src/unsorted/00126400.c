/* UNIT 0x00126400 0x001264D8 */
typedef struct Pair { long value; } __attribute__((packed)) Pair;
typedef struct Data { unsigned int x0; } Data;
typedef struct Link { int x0; Data **x4; } Link;
typedef struct Obj { char pad0[8]; int x8; char padC[0x34]; int x40,x44,x48,x4C,x50,x54,x58,x5C,x60; char pad64[0x68]; float xCC; char padD0[0x60]; Link *x130; int x134; char pad138[0x14]; Pair x14C,x154; } Obj;
extern Pair D_0029C210[], D_0029C218[];
extern float func_00222B68(float, float);
void func_00126400(Obj *obj) {
 Data *data;
 obj->x14C = D_0029C210[0];
 obj->x154 = D_0029C218[0];
 obj->x8 = 1;
 obj->x40 = 1;
 obj->x44 = 1;
 obj->x48 = 1;
 obj->x4C = 9;
 obj->x54 = 8;
 obj->x58 = 29;
 obj->x5C = 30;
 obj->x50 = 3;
 data = *obj->x130->x4;
 if ((data->x0 >> 18) == 0x104A) obj->x60 = 27;
 else obj->x60 = 28;
 obj->xCC = func_00222B68(-0.3490658402442932f, 0.3490658402442932f);
 obj->x134 = 0;
}
