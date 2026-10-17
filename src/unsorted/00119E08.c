/* UNIT 0x00119E08 0x00119E40 */
typedef struct Global { char pad0[0x1006C]; void *x1006C; } Global;
extern Global D_0029EA20;
extern char D_00283980[];
void func_00119E08(int arg0, int arg1) {
    if (arg1 == 0xFFFF && arg0 != 0) D_0029EA20.x1006C = D_00283980;
}
