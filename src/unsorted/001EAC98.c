/* UNIT 0x001EAC98 0x001EACF0 */
struct Obj { char pad[0xf0]; unsigned char xf0; char padf1[0x5f]; int x150; };
extern void func_001EAF18(void *, unsigned int *, void *);
void func_001EAC98(struct Obj *arg0, unsigned int *arg1, void *pos) {
    arg0->xf0 = 1;
    func_001EAF18((char *)arg0 + ((*arg1 >> 18) - 6000) * 80, arg1, pos);
    arg0->x150++;
}

