/* UNIT 0x00235010 0x00235060 */
extern char D_002991F0[];
class Obj {
    char pad0[0x5FC];
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6(char *, int, int, int, int);
    void func_00235010(int, int, int, int);
};
void Obj::func_00235010(int a, int b, int c, int d) {
    v6(D_002991F0, a, b, c, d);
}
