/* UNIT 0x0015E0C0 0x0015E110 */
struct State { char pad[0x12D]; char x12D; };
struct Other { char pad[0x354]; int x354; int x358; char x35C; };
extern struct State D_003F52D0;
extern struct Other D_004B25A0;
extern void func_001B5AF0(void *, int, int);
void func_0015E0C0(void) {
    func_001B5AF0(&D_003F52D0, 1, 1);
    D_003F52D0.x12D = 1;
    D_004B25A0.x354 = 1;
    D_004B25A0.x35C = 0;
}
