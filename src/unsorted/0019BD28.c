/* UNIT 0x0019BD28 0x0019BDA0 */
typedef struct Obj { char pad[0x11]; char x11,x12,x13; char pad1[0x38]; int x4C; } Obj;
extern char D_0026F938[],D_0043CD50[];
extern void func_0019BB30(Obj *,void *);
extern void func_0019B100(void *);
void func_0019BD28(Obj *o) {
 int i; char *p;
 o->x12=0; o->x4C=0; o->x13=0; o->x11=1;
 func_0019BB30(o,D_0026F938);
 func_0019B100(D_0043CD50);
 p=(char *)o+0x4b; i=39;
 do { *p=1; i--; p--; } while (i>=0);
}
