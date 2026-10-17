/* UNIT 0x001517A0 0x001517D0 */
struct Obj {
    char pad0[0xA30];
    int count;
    char padA34[0x10];
    int xA44[15];
    int xA80[15];
};
void func_001517A0(struct Obj *arg0, int arg1, int arg2) {
    arg0->xA44[arg0->count] = arg1;
    arg0->xA80[arg0->count] = arg2;
    arg0->count++;
}
