/* UNIT 0x001FA300 0x001FA370 */
extern int D_00275EA0[], D_00275E38[], D_00275E08[];
int func_001FA300(int kind, int index) {
 switch(kind) {
 case 1: return D_00275EA0[index];
 case 2: return D_00275E38[index];
 case 3: return D_00275E08[index];
 default: return 0;
 }
}
