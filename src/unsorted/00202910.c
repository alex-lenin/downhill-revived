/* UNIT 0x00202910 0x00202950 */
typedef struct Obj {
    char pad0[0x10];
    volatile int x10;
} Obj;

extern int D_00279434[];
extern Obj D_004CD380;
extern int func_00247F08(void);

int func_00202910(void)
{
    if (D_00279434[0] == 0) {
        return func_00247F08();
    }
    return D_004CD380.x10;
}
