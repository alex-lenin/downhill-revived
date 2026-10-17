/* UNIT 0x001713C8 0x00171468 */
struct Result { int x0,x4; float x8; };
struct Obj { char pad[0x29A8]; float x29A8; char pad2[0x18]; int x29C4; };
struct Global { char pad[0x50198]; float x50198; };
extern struct Global D_002AF2C0;
extern char D_003AD8B0[];
extern int func_00163C58(void *, int, int, float);
extern struct Result *func_00163D40(void *, int);
struct Result *func_001713C8(struct Obj *obj) {
 struct Result *result = 0;
 float limit = D_002AF2C0.x50198 - 200.0f;
 float value;
 int index;
 __asm__ volatile("min.s %0,%1,%2" : "=&f"(value) : "f"(obj->x29A8), "f"(limit));
 obj->x29A8 = value;
 index = func_00163C58(D_003AD8B0, obj->x29C4, 0, value);
 obj->x29C4 = index;
 if(index >= 0) { result = func_00163D40(D_003AD8B0,index); obj->x29A8 = result->x8; }
 return result;
}
