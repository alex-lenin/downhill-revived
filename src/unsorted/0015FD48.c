/* UNIT 0x0015FD48 0x0015FDA8 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union { u128 bits; float v[4]; } Vector;
typedef struct { char pad[0x134]; int x134; } Obj;
extern Obj D_003F52D0;
extern void func_0018FFE0(Obj *, int, Vector *);
extern void func_0018DC20(void *);
void func_0015FD48(void *obj) {
    Vector vector;
    vector.bits = 0;
    vector.v[1] = 35.0f;
    vector.v[2] = 16.0f;
    func_0018FFE0(&D_003F52D0, D_003F52D0.x134, &vector);
    func_0018DC20(obj);
}
