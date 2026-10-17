/* UNIT 0x0023D060 0x0023D098 */
typedef struct { int x0; char pad4[0xEC]; } Obj;
extern Obj D_00528360[];
void func_0023D060(int value) {
    int count = 5;
    Obj *p = D_00528360;
    p += count;
    do { p->x0=value; count--; p--; } while(count >= 0);
}
