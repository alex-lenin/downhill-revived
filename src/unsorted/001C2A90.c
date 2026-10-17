/* UNIT 0x001C2A90 0x001C2AE8 */
typedef struct { char pad0[0x288]; int x288; char pad28C[0x14]; int x2A0; } State;
extern State D_004B25A0;
extern int func_00182290(int);
int func_001C2A90(void) {
    if (D_004B25A0.x288 != 1) return 0;
    if (func_00182290(D_004B25A0.x2A0) == 0 || D_004B25A0.x2A0 == 0x19) return 1;
    return 0;
}
