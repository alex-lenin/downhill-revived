/* UNIT 0x001EABE0 0x001EAC40 */
typedef struct Obj { char pad[0x18]; float x18; } Obj;
extern int func_001E64F0(Obj *);
extern void func_001E6420(Obj *,float), func_001E67A8(Obj *);
void func_001EABE0(char **p) {
 Obj *q=(Obj *)(*p+0x6610);
 if(!func_001E64F0(q)) { func_001E6420(q,(float)(int)q->x18); func_001E67A8(q); }
}
