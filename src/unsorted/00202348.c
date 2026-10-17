/* UNIT 0x00202348 0x00202388 */
typedef struct Obj {
    int x0;
} Obj;

extern Obj *D_00279448[];
extern int D_00279468[];
extern int func_002013F8(void);

int func_00202348(void)
{
    D_00279448[D_00279468[0]]->x0++;
    return func_002013F8();
}
