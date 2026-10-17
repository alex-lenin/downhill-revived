/* UNIT 0x00194898 0x00194940 */
typedef struct Rider { char p[0x798C]; int x798C; } Rider;
typedef struct State { char p[0x50130]; Rider *x50130, *x50134; } State;
typedef struct Settings { char p[0x10C]; int x10C; char p2[0x2A0-0x110]; int x2A0; } Settings;
extern State D_002AF2C0;
extern Settings D_004B25A0;
extern int func_00194430(void *, int, int);
void func_00194898(void *obj) {
 int sum = func_00194430(obj,D_002AF2C0.x50130->x798C,D_004B25A0.x2A0);
 int *slot;
 sum += func_00194430(obj,D_002AF2C0.x50134->x798C,D_004B25A0.x2A0);
 slot = (int *)((char *)obj + (D_004B25A0.x10C-4)*0x50 + 0x11A88);
 slot[0x44/4] += sum;
}
