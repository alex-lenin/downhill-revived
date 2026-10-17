/* UNIT 0x00143C50 0x00143CB0 */
typedef struct { char pad[0x68]; float x68, x6C; } Sub;
typedef struct { int x0; char pad[0xC]; Sub sub; } Obj;
extern void func_001430E8(Sub *p, unsigned int *w);
extern void func_00143B98(Sub *p, int type);
void func_00143C50(Obj *obj, unsigned int *word) {
    func_001430E8(&obj->sub, word);
    func_00143B98(&obj->sub, *word >> 18);
    if (obj->x0 <= 0) obj->x0 = 1;
}
