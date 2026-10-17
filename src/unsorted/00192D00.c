/* UNIT 0x00192D00 0x00192D50 */
/* The retail function is entered two words into its range: the first two
   words are a frame-header pair (`addiu $sp,$sp,16' / `nop', word encoding
   0x27BD0010). The code pointer in the table at 0x00287384 is 0x00192D08 and
   nothing references 0x00192D00, so the pair is never executed; cc1 does not
   emit it from C. It is provided here as data words in .text, with the C body
   emitted immediately after them. */
__attribute__((section(".text"))) static const unsigned int prefunc[2] = { 0x27BD0010, 0x00000000 };

typedef struct {
    char pad0[0x230];
    void *x230;
    char x234[1];
} Obj;

extern void func_00126E08(void *, int, int);
extern void func_001BDA58(void *, void *, int, int);

void func_00192D00_body(Obj *obj, int a1, int a2)
{
    if (obj->x230) {
        func_00126E08(obj->x230, a1, a2);
        func_001BDA58(obj->x234, obj, 0x14, 0x8c);
    }
}
