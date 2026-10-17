/* UNIT 0x00138828 0x00138868 */
typedef unsigned int Quad __attribute__((mode(TI)));
extern Quad D_003613E0;
extern Quad D_003613D0;
struct Obj { char pad0[0x31765]; char x31765; };
void func_00138828(struct Obj *obj, Quad *value) {
    Quad input = *value;
    volatile Quad saved = input;
    Quad previous;
    obj->x31765 = 1;
    previous = D_003613E0;
    D_003613E0 = input;
    D_003613D0 = previous;
}
