/* UNIT 0x0014EA70 0x0014EB08 */
struct State { char pad0[0x10c]; int x10c; char pad110[0x178]; int x288; int x28c; int x290; };
extern struct State D_004B25A0;
extern char D_004225A0[];
extern int func_00194A10(void *, int);
extern int func_00194C80(void *, int);
int func_0014EA70(int *obj) {
 int result;
 if (*obj < 0) return 0;
 if (D_004B25A0.x288 == 2) result=func_00194A10(D_004225A0,D_004B25A0.x10c);
 else result=func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5)-1)*0x694,D_004B25A0.x10c);
 return result == *obj + 1;
}
