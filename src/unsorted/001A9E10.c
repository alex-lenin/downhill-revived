/* UNIT 0x001A9E10 0x001A9EA8 */
typedef struct Obj { char p0[0x28]; int x28; char p1[4]; int x30; char p2[0x13]; char x47; char p3[0x974]; int x9BC,x9C0,x9C4; } Obj;
extern int D_004B25A0[]; extern void func_00220018(int,int,int,int); extern void func_00220198(int,int);
void func_001A9E10(Obj *obj) {int i; obj->x9C4=0x1C20;obj->x9C0=-1;obj->x9BC=0;obj->x30=0;obj->x28=0;obj->x47=0;for(i=0;i<D_004B25A0[0x288/4];i++){func_00220018(i,0,0,0);func_00220198(i,0);}}
