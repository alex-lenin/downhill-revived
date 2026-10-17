/* UNIT 0x00146038 0x00146060 */
typedef struct Obj {
    void *vtbl;
    char pad4[0x201C];
} Obj;

extern char D_00285780[];
extern Obj D_00370630[];

/* static initializer: sets the vtable of a global object */
void func_00146038(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        D_00370630[0].vtbl = D_00285780;
    }
}
