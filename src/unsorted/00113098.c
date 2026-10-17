/* UNIT 0x00113098 0x00113170 */
typedef struct Pos { char pad0[0x18]; float x18; } Pos;
typedef struct Game { char pad0[0x792C]; Pos *x792C; } Game;
typedef struct Obj { Game *x0; int x4; Game *x8; char padC[0x3FC]; int x408; char pad40C[0x30]; int x43C; unsigned long x440; } Obj;
extern int D_0043EEC8[];
extern void func_00112EF8(Obj *, int);
extern void func_00113170(Obj *);
extern float fabsf(float);
void func_00113098(Obj *obj) {
 int divisor = obj->x43C;
 int limit = 240;
 float b;
 float a;
 float d;
 float distance;
 float threshold = 20.0f;
 if (divisor) limit = 240 - D_0043EEC8[0] * 240 / divisor;
 if (limit < obj->x408) { func_00112EF8(obj, limit); return; }
 b = obj->x8->x792C->x18;
 a = obj->x0->x792C->x18;
 d = b - a;
 distance = fabsf(d);
 __asm__ volatile ("Lafterabs_113098:");
 if (threshold > distance) goto flags;
 if (!(d > -10.0f)) goto flags;
 func_00113170(obj);
 return;
flags:
 obj->x440 |= (unsigned long)0x8000 << 19;
}
