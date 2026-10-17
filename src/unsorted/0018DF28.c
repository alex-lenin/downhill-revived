/* UNIT 0x0018DF28 0x0018DFA8 */
extern int D_003F5400[];
extern int func_00238F48(void);
extern int func_002205A0(int);
extern int func_00220218(int);
extern void func_0021F820(void);
int func_0018DF28(int index, int check) {
 int result;
 if (check && func_00238F48() < 240) return 0;
 if (index < 0) index = D_003F5400[0];
 result = (func_00220218(func_002205A0(index)) ^ 1) & 255;
 if (index == 0 && result) func_0021F820();
 return result;
}
