/* UNIT 0x00144778 0x001447A0 */
typedef struct Obj {
    void *vtbl;
    char pad4[0x201C];
} Obj;

extern char D_00285780[];
extern Obj D_0036D030[];

/* static initializer: sets the vtable of a global object */
void func_00144778(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        D_0036D030[0].vtbl = D_00285780;
    }
}
