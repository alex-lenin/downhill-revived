/* UNIT 0x001E3810 0x001E3938 */
extern int D_004225A0[];
extern int D_004C37A0[];
extern int D_004C4210[];
extern int D_003F52D0[];

extern int func_00193F38(void *);
extern int func_001F8440(int);
extern int func_001E3060(void *, int);
extern void func_001E3568(void *, int);
extern void func_00190688(void *);

void func_001E3810(int unused, int target)
{
    int i;
    int count;
    int selected;
    unsigned int mask;

    i = 0;
    count = 0;
    selected = 0;
    mask = func_00193F38(D_004225A0);
    for (; func_001F8440(i) >= 0; i++) {
        if ((1 << func_001F8440(i)) & mask) {
            if (func_001E3060(D_004C37A0, func_001F8440(i)) != 0) {
                continue;
            }
            if (func_001F8440(i) == 0x15) {
                continue;
            }
            if (func_001F8440(i) == 0x17) {
                continue;
            }
            if (count == target) {
                selected = func_001F8440(i);
            }
            count++;
        }
    }
    func_001E3568(D_004C4210, selected);
    func_00190688(D_003F52D0);
}
