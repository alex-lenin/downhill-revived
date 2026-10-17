/* UNIT 0x002079C8 0x00207A10 */
struct State { char pad[0x488]; int count; char entries[1][32]; };
extern struct State D_002794A8;
extern int func_00254544(void *, void *);
int func_002079C8(void *arg0) {
    int result = func_00254544(D_002794A8.entries[D_002794A8.count], arg0);
    D_002794A8.count++;
    return result;
}
