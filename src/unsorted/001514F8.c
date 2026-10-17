/* UNIT 0x001514F8 0x001516D8 .rodata=0x28A260:0x28A280 */
typedef struct Obj { char pad[0xABC]; unsigned int kind; } Obj;
extern char D_004225A0[];
typedef struct Row { int pad; int values[3]; } Row;
extern Row D_0026E6B0[];
extern int func_00194770(void *,int);
int func_001514F8(Obj *obj, char *flag) {
 int a,b,c,d,e,f;
 int result = 0;
 int level;
 *flag = 0;
 a = func_00194770(D_004225A0,0);
 b = func_00194770(D_004225A0,1);
 c = func_00194770(D_004225A0,2);
 d = func_00194770(D_004225A0,3);
 e = func_00194770(D_004225A0,4);
 f = func_00194770(D_004225A0,5);
 a = a >= 4 ? 3 : a;
 b = b >= 4 ? 3 : b;
 c = c >= 4 ? 3 : c;
 d = d >= 4 ? 3 : d;
 e = e >= 4 ? 3 : e;
 f = f < 4 ? f : 3;
 switch (obj->kind) {
 case 0: if (a == 1) *flag = a; result = D_0026E6B0[obj->kind].values[a-1]; break;
 case 1: if (b == 1) *flag = b; result = D_0026E6B0[obj->kind].values[b-1]; break;
 case 2: if (c == 1) *flag = c; result = D_0026E6B0[obj->kind].values[c-1]; break;
 case 3: if (d == 1) *flag = d; result = D_0026E6B0[obj->kind].values[d-1]; break;
 case 4: if (e == 1) *flag = e; result = D_0026E6B0[obj->kind].values[e-1]; break;
 case 5: if (f == 1) *flag = f; result = D_0026E6B0[obj->kind].values[f-1]; break;
 }
 return result;
}
