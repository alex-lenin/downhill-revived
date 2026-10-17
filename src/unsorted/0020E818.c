/* UNIT 0x0020E818 0x0020E860 */
typedef struct {
    char pad[0x1c];
    int x1c;
} Obj;
extern int D_004F321C[];
extern Obj D_004F32E0[];
void func_0020E818(void) {
    int i;
    int value;
    D_004F321C[0]=1;
    value=1;
    for(i=5;
    i>=0;
    i--) D_004F32E0[i].x1c=value;
}
